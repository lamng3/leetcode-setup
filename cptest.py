#!/usr/bin/env python3
"""Compile and test a LeetCode C++ solution against example test cases."""

import sys
import os
import json
import re
import html
import subprocess
import tempfile
import urllib.request

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
SOLVE_DIR = os.path.join(SCRIPT_DIR, "solve")
TESTS_DIR = os.path.join(SCRIPT_DIR, "tests")
LEETCODE_GRAPHQL = "https://leetcode.com/graphql"

# ── LeetCode API helpers ──────────────────────────────────────────────

def search_problem(number: str) -> str:
    query = {
        "query": """
        query problemsetQuestionList($categorySlug: String, $limit: Int, $skip: Int, $filters: QuestionListFilterInput) {
            problemsetQuestionList: questionList(categorySlug: $categorySlug, limit: $limit, skip: $skip, filters: $filters) {
                questions: data { titleSlug frontendQuestionId: questionFrontendId }
            }
        }""",
        "variables": {
            "categorySlug": "all-code-essentials",
            "limit": 5,
            "skip": 0,
            "filters": {"searchKeywords": number},
        },
    }
    req = urllib.request.Request(
        LEETCODE_GRAPHQL,
        data=json.dumps(query).encode(),
        headers={"Content-Type": "application/json", "Referer": "https://leetcode.com", "User-Agent": "Mozilla/5.0"},
    )
    with urllib.request.urlopen(req) as resp:
        data = json.loads(resp.read())
    questions = data["data"]["problemsetQuestionList"]["questions"]
    for q in questions:
        if q["frontendQuestionId"] == number:
            return q["titleSlug"]
    if questions:
        return questions[0]["titleSlug"]
    print(f"Error: No problem found for number {number}")
    sys.exit(1)


def fetch_problem(slug: str) -> dict:
    query = {
        "query": """
        query questionData($titleSlug: String!) {
            question(titleSlug: $titleSlug) {
                questionFrontendId
                title
                titleSlug
                content
                metaData
                exampleTestcaseList
                codeSnippets { langSlug code }
            }
        }""",
        "variables": {"titleSlug": slug},
    }
    req = urllib.request.Request(
        LEETCODE_GRAPHQL,
        data=json.dumps(query).encode(),
        headers={"Content-Type": "application/json", "Referer": "https://leetcode.com", "User-Agent": "Mozilla/5.0"},
    )
    with urllib.request.urlopen(req) as resp:
        data = json.loads(resp.read())
    return data["data"]["question"]


def parse_expected_outputs(content_html: str) -> list[str]:
    """Extract expected outputs from the problem description HTML."""
    text = html.unescape(content_html)
    outputs = []

    # Pattern 1: <strong>Output:</strong> value on same line
    for m in re.finditer(r'<strong>\s*Output\s*:?\s*</strong>\s*:?\s*(?:<code>)?\s*(.+?)(?:</code>)?\s*(?:</p>|<br|</pre>|\n)', text, re.IGNORECASE):
        val = re.sub(r'<[^>]+>', '', m.group(1)).strip()
        if val:
            outputs.append(val)

    if outputs:
        return outputs

    # Pattern 2: <strong>Output</strong> with value on next line
    for m in re.finditer(r'<strong>\s*Output\s*:?\s*</strong>\s*:?\s*\n\s*(.+?)(?:\n|<)', text, re.IGNORECASE):
        val = re.sub(r'<[^>]+>', '', m.group(1)).strip()
        if val:
            outputs.append(val)

    if outputs:
        return outputs

    # Pattern 3: Output: value on its own line (in <pre> blocks)
    for m in re.finditer(r'Output\s*:?\s*(.+)', text):
        val = re.sub(r'<[^>]+>', '', m.group(1)).strip()
        if val:
            outputs.append(val)

    return outputs


# ── Test case storage ─────────────────────────────────────────────────

def load_or_fetch_tests(problem_number: str) -> dict:
    """Load cached test cases or fetch from LeetCode."""
    test_file = os.path.join(TESTS_DIR, f"{problem_number}.json")

    if os.path.exists(test_file):
        with open(test_file) as f:
            return json.load(f)

    print(f"Fetching test cases for problem {problem_number}...")
    slug = search_problem(problem_number)
    problem = fetch_problem(slug)

    meta = json.loads(problem["metaData"])
    inputs_list = problem["exampleTestcaseList"]
    expected_list = parse_expected_outputs(problem["content"])

    test_cases = []
    for i, raw_input in enumerate(inputs_list):
        lines = raw_input.strip().splitlines()
        tc = {
            "input_lines": lines,
            "expected": expected_list[i] if i < len(expected_list) else None,
        }
        test_cases.append(tc)

    # Detect "any order" from problem description
    content = problem["content"] or ""
    content_text = re.sub(r'<[^>]+>', ' ', content)
    any_order = bool(re.search(r'(return|answer).{0,30}in\s+any\s+order', content_text, re.IGNORECASE))

    data = {
        "number": problem_number,
        "title": problem["title"],
        "slug": problem["titleSlug"],
        "metaData": meta,
        "anyOrder": any_order,
        "testCases": test_cases,
    }

    os.makedirs(TESTS_DIR, exist_ok=True)
    with open(test_file, "w") as f:
        json.dump(data, f, indent=2)
    print(f"Cached test cases to {test_file}")
    return data


# ── C++ code generation ──────────────────────────────────────────────

# Map LeetCode type strings to C++ types and their parsers
TYPE_MAP = {
    "integer":    ("int",                 "parse_int(line)"),
    "long":       ("long long",           "parse_ll(line)"),
    "double":     ("double",              "parse_double(line)"),
    "float":      ("double",              "parse_double(line)"),
    "boolean":    ("bool",                "parse_bool(line)"),
    "string":     ("string",              "parse_string(line)"),
    "character":  ("char",                "parse_char(line)"),
    "integer[]":  ("vector<int>",         "parse_vec_int(line)"),
    "long[]":     ("vector<long long>",   "parse_vec_ll(line)"),
    "double[]":   ("vector<double>",      "parse_vec_double(line)"),
    "string[]":   ("vector<string>",      "parse_vec_string(line)"),
    "boolean[]":  ("vector<bool>",        "parse_vec_bool(line)"),
    "integer[][]":("vector<vector<int>>", "parse_vec_vec_int(line)"),
    "string[][]": ("vector<vector<string>>","parse_vec_vec_string(line)"),
    "list<integer>":  ("vector<int>",     "parse_vec_int(line)"),
    "list<string>":   ("vector<string>",  "parse_vec_string(line)"),
    "list<boolean>":  ("vector<bool>",    "parse_vec_bool(line)"),
    "list<list<integer>>":("vector<vector<int>>", "parse_vec_vec_int(line)"),
    "list<list<string>>": ("vector<vector<string>>","parse_vec_vec_string(line)"),
    "TreeNode":   ("TreeNode*",           "parse_tree(line)"),
    "TreeNode*":  ("TreeNode*",           "parse_tree(line)"),
    "ListNode":   ("ListNode*",           "parse_listnode(line)"),
    "ListNode*":  ("ListNode*",           "parse_listnode(line)"),
}

def cpp_type(lc_type: str) -> str:
    if lc_type in TYPE_MAP:
        return TYPE_MAP[lc_type][0]
    return lc_type

def cpp_parser(lc_type: str) -> str:
    if lc_type in TYPE_MAP:
        return TYPE_MAP[lc_type][1]
    return f"/* UNKNOWN TYPE: {lc_type} */"

def cpp_printer(lc_type: str, expr: str) -> str:
    """Generate code to print a value in LeetCode-compatible format."""
    if lc_type in ("integer", "long", "double", "float"):
        return f'cout << {expr};'
    if lc_type == "boolean":
        return f'cout << ({expr} ? "true" : "false");'
    if lc_type == "string":
        return f'cout << "\\"" << {expr} << "\\"";'
    if lc_type == "character":
        return f'cout << "\\"" << {expr} << "\\"";'
    if lc_type.endswith("[][]") or lc_type.startswith("list<list<"):
        return f'print_2d({expr});'
    if lc_type.endswith("[]") or lc_type.startswith("list<"):
        inner = lc_type.replace("[]", "").replace("list<", "").replace(">", "")
        if inner in ("string",):
            return f'print_vec_string({expr});'
        if inner == "boolean":
            return f'print_vec_bool({expr});'
        return f'print_vec({expr});'
    if lc_type in ("TreeNode", "TreeNode*"):
        return f'print_tree({expr});'
    if lc_type in ("ListNode", "ListNode*"):
        return f'print_listnode({expr});'
    # fallback: use the template's operator<< for vectors, else raw cout
    return f'cout << {expr};'


PARSE_HELPERS = r'''
// ── Parsing helpers ──────────────────────────────────────────────────
string trim(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

int parse_int(const string& s) { return stoi(trim(s)); }
long long parse_ll(const string& s) { return stoll(trim(s)); }
double parse_double(const string& s) { return stod(trim(s)); }
bool parse_bool(const string& s) {
    string t = trim(s);
    return t == "true" || t == "1";
}
string parse_string(const string& s) {
    string t = trim(s);
    if (t.size() >= 2 && t.front() == '"' && t.back() == '"')
        t = t.substr(1, t.size() - 2);
    return t;
}
char parse_char(const string& s) {
    string t = parse_string(s);
    return t.empty() ? '\0' : t[0];
}

// Tokenize a JSON-like array string into top-level elements
vector<string> tokenize_array(const string& s) {
    string t = trim(s);
    if (t.size() < 2 || t.front() != '[' || t.back() != ']') return {t};
    t = t.substr(1, t.size() - 2);
    vector<string> tokens;
    int depth = 0;
    bool in_str = false;
    string cur;
    for (size_t i = 0; i < t.size(); i++) {
        char c = t[i];
        if (c == '"' && (i == 0 || t[i-1] != '\\')) { in_str = !in_str; cur += c; continue; }
        if (in_str) { cur += c; continue; }
        if (c == '[' || c == '{') { depth++; cur += c; continue; }
        if (c == ']' || c == '}') { depth--; cur += c; continue; }
        if (c == ',' && depth == 0) { tokens.push_back(trim(cur)); cur.clear(); continue; }
        cur += c;
    }
    if (!cur.empty()) tokens.push_back(trim(cur));
    return tokens;
}

vector<int> parse_vec_int(const string& s) {
    auto toks = tokenize_array(s);
    vector<int> v;
    for (auto& t : toks) if (!t.empty()) v.push_back(stoi(t));
    return v;
}
vector<long long> parse_vec_ll(const string& s) {
    auto toks = tokenize_array(s);
    vector<long long> v;
    for (auto& t : toks) if (!t.empty()) v.push_back(stoll(t));
    return v;
}
vector<double> parse_vec_double(const string& s) {
    auto toks = tokenize_array(s);
    vector<double> v;
    for (auto& t : toks) if (!t.empty()) v.push_back(stod(t));
    return v;
}
vector<bool> parse_vec_bool(const string& s) {
    auto toks = tokenize_array(s);
    vector<bool> v;
    for (auto& t : toks) v.push_back(trim(t) == "true" || trim(t) == "1");
    return v;
}
vector<string> parse_vec_string(const string& s) {
    auto toks = tokenize_array(s);
    vector<string> v;
    for (auto& t : toks) v.push_back(parse_string(t));
    return v;
}
vector<vector<int>> parse_vec_vec_int(const string& s) {
    auto toks = tokenize_array(s);
    vector<vector<int>> v;
    for (auto& t : toks) v.push_back(parse_vec_int(t));
    return v;
}
vector<vector<string>> parse_vec_vec_string(const string& s) {
    auto toks = tokenize_array(s);
    vector<vector<string>> v;
    for (auto& t : toks) v.push_back(parse_vec_string(t));
    return v;
}

// ── Tree / ListNode parsing ──────────────────────────────────────────
#ifdef NEED_TREE
#ifndef LOCAL
// Fallback for old solution files without struct definition
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};
#endif
TreeNode* parse_tree(const string& s) {
    auto toks = tokenize_array(s);
    if (toks.empty() || toks[0] == "null") return nullptr;
    TreeNode* root = new TreeNode(stoi(toks[0]));
    queue<TreeNode*> q;
    q.push(root);
    int i = 1;
    while (!q.empty() && i < (int)toks.size()) {
        TreeNode* node = q.front(); q.pop();
        if (i < (int)toks.size() && trim(toks[i]) != "null") {
            node->left = new TreeNode(stoi(toks[i]));
            q.push(node->left);
        }
        i++;
        if (i < (int)toks.size() && trim(toks[i]) != "null") {
            node->right = new TreeNode(stoi(toks[i]));
            q.push(node->right);
        }
        i++;
    }
    return root;
}
void print_tree(TreeNode* root) {
    cout << "[";
    queue<TreeNode*> q;
    q.push(root);
    bool first = true;
    // BFS, track last non-null to trim trailing nulls
    vector<string> parts;
    while (!q.empty()) {
        auto* n = q.front(); q.pop();
        if (n) {
            parts.push_back(to_string(n->val));
            q.push(n->left);
            q.push(n->right);
        } else {
            parts.push_back("null");
        }
    }
    // trim trailing nulls
    while (!parts.empty() && parts.back() == "null") parts.pop_back();
    for (int i = 0; i < (int)parts.size(); i++) {
        if (i) cout << ",";
        cout << parts[i];
    }
    cout << "]";
}
#endif

#ifdef NEED_LISTNODE
#ifndef LOCAL
// Fallback for old solution files without struct definition
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};
#endif
ListNode* parse_listnode(const string& s) {
    auto v = parse_vec_int(s);
    ListNode dummy(0);
    ListNode* cur = &dummy;
    for (int x : v) { cur->next = new ListNode(x); cur = cur->next; }
    return dummy.next;
}
void print_listnode(ListNode* head) {
    cout << "[";
    for (ListNode* p = head; p; p = p->next) {
        if (p != head) cout << ",";
        cout << p->val;
    }
    cout << "]";
}
#endif

// ── Printing helpers ─────────────────────────────────────────────────
template<typename T>
void print_vec(const vector<T>& v) {
    cout << "[";
    for (int i = 0; i < (int)v.size(); i++) {
        if (i) cout << ",";
        cout << v[i];
    }
    cout << "]";
}
void print_vec_string(const vector<string>& v) {
    cout << "[";
    for (int i = 0; i < (int)v.size(); i++) {
        if (i) cout << ",";
        cout << "\"" << v[i] << "\"";
    }
    cout << "]";
}
void print_vec_bool(const vector<bool>& v) {
    cout << "[";
    for (int i = 0; i < (int)v.size(); i++) {
        if (i) cout << ",";
        cout << (v[i] ? "true" : "false");
    }
    cout << "]";
}
template<typename T>
void print_2d(const vector<vector<T>>& v) {
    cout << "[";
    for (int i = 0; i < (int)v.size(); i++) {
        if (i) cout << ",";
        print_vec(v[i]);
    }
    cout << "]";
}
'''


def generate_runner_cpp(solution_path: str, meta: dict, needs_tree: bool, needs_listnode: bool) -> str:
    """Generate a C++ test runner that includes the solution and calls the method."""
    method_name = meta["name"]
    params = meta["params"]
    ret_type = meta["return"]["type"]

    # Check if method modifies input in-place (void return)
    is_void = ret_type == "void"

    # Build parameter declarations and parsing
    param_decls = []
    param_parse = []
    param_names = []
    for i, p in enumerate(params):
        ctype = cpp_type(p["type"])
        pname = p["name"]
        parser = cpp_parser(p["type"])
        param_decls.append(f'    {ctype} {pname};')
        param_parse.append(f'    getline(cin, line); {pname} = {parser};')
        param_names.append(pname)

    call_args = ", ".join(param_names)
    decls = "\n".join(param_decls)
    parsing = "\n".join(param_parse)

    defines = ""
    if needs_tree:
        defines += "#define NEED_TREE\n"
    if needs_listnode:
        defines += "#define NEED_LISTNODE\n"

    if is_void:
        # For void methods, the output is typically a modified input parameter
        # Usually the first array/vector parameter is the one modified
        output_param = param_names[0] if param_names else ""
        output_type = params[0]["type"] if params else "integer[]"
        printer = cpp_printer(output_type, output_param)
        call_and_print = f'''    sol.{method_name}({call_args});
    {printer}'''
    else:
        cpp_ret = cpp_type(ret_type)
        printer = cpp_printer(ret_type, "result")
        call_and_print = f'''    {cpp_ret} result = sol.{method_name}({call_args});
    {printer}'''

    return f'''// Auto-generated test runner
// Provide dbg() fallback for old solution files without it
#ifdef LOCAL
#include <iostream>
inline void _cptest_dbg() {{ std::cerr << std::endl; }}
template<typename T, typename... A>
void _cptest_dbg(T t, A... a) {{ std::cerr << " " << t; if constexpr(sizeof...(a)) std::cerr << ","; _cptest_dbg(a...); }}
#define dbg(...) std::cerr << "\\033[35m[" << #__VA_ARGS__ << "]\\033[0m:", _cptest_dbg(__VA_ARGS__)
#else
#define dbg(...)
#endif
// Solution may redefine dbg — allow it
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmacro-redefined"
{defines}#include "{solution_path}"
#pragma GCC diagnostic pop
{PARSE_HELPERS}

int main() {{
    string line;
{decls}
{parsing}

    Solution sol;
{call_and_print}
    cout << endl;
    return 0;
}}
'''


# ── Design problem runner ─────────────────────────────────────────────

def is_design_problem(meta: dict) -> bool:
    return "classname" in meta


def generate_design_runner_cpp(solution_path: str, meta: dict, needs_tree: bool, needs_listnode: bool) -> str:
    """Generate a C++ test runner for design (class-based) problems."""
    classname = meta["classname"]
    constructor = meta["constructor"]
    methods = meta["methods"]

    defines = ""
    if needs_tree:
        defines += "#define NEED_TREE\n"
    if needs_listnode:
        defines += "#define NEED_LISTNODE\n"

    # Generate constructor argument parsing
    ctor_params = constructor.get("params", [])
    ctor_parse_lines = []
    ctor_args = []
    for j, p in enumerate(ctor_params):
        ctype = cpp_type(p["type"])
        pname = f"ctor_{p['name']}"
        parser = cpp_parser(p["type"]).replace("line", f"ctor_toks[{j}]")
        ctor_parse_lines.append(f'                {ctype} {pname} = {parser};')
        ctor_args.append(pname)
    ctor_parse = "\n".join(ctor_parse_lines)
    ctor_call = ", ".join(ctor_args)

    # Generate method dispatch
    method_cases = []
    for m in methods:
        mname = m["name"]
        mparams = m.get("params", [])
        mret = m["return"]["type"]

        # Parse args for this method
        arg_parse_lines = []
        arg_names = []
        for j, p in enumerate(mparams):
            ctype = cpp_type(p["type"])
            pname = f"arg_{p['name']}"
            parser = cpp_parser(p["type"]).replace("line", f"m_toks[{j}]")
            arg_parse_lines.append(f'                {ctype} {pname} = {parser};')
            arg_names.append(pname)
        arg_parse = "\n".join(arg_parse_lines)
        arg_call = ", ".join(arg_names)

        if mret == "void":
            case = f'''            if (methods[i] == "{mname}") {{
                auto m_toks = tokenize_array(args_toks[i]);
{arg_parse}
                obj->{mname}({arg_call});
                cout << "null";
            }}'''
        else:
            printer_expr = cpp_printer(mret, "ret")
            case = f'''            if (methods[i] == "{mname}") {{
                auto m_toks = tokenize_array(args_toks[i]);
{arg_parse}
                {cpp_type(mret)} ret = obj->{mname}({arg_call});
                {printer_expr}
            }}'''
        method_cases.append(case)

    dispatch = " else ".join(method_cases)

    return f'''// Auto-generated test runner (design problem)
#ifdef LOCAL
#include <iostream>
inline void _cptest_dbg() {{ std::cerr << std::endl; }}
template<typename T, typename... A>
void _cptest_dbg(T t, A... a) {{ std::cerr << " " << t; if constexpr(sizeof...(a)) std::cerr << ","; _cptest_dbg(a...); }}
#define dbg(...) std::cerr << "\\033[35m[" << #__VA_ARGS__ << "]\\033[0m:", _cptest_dbg(__VA_ARGS__)
#else
#define dbg(...)
#endif
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmacro-redefined"
{defines}#include "{solution_path}"
#pragma GCC diagnostic pop
{PARSE_HELPERS}

int main() {{
    string methods_line, args_line;
    getline(cin, methods_line);
    getline(cin, args_line);

    auto methods = parse_vec_string(methods_line);
    auto args_toks = tokenize_array(args_line);

    {classname}* obj = nullptr;
    cout << "[";
    for (int i = 0; i < (int)methods.size(); i++) {{
        if (i) cout << ",";
        if (methods[i] == "{classname}") {{
            auto ctor_toks = tokenize_array(args_toks[i]);
{ctor_parse}
            obj = new {classname}({ctor_call});
            cout << "null";
        }} else {dispatch}
    }}
    cout << "]" << endl;
    delete obj;
    return 0;
}}
'''


# ── Compilation and testing ───────────────────────────────────────────

def needs_special_types(meta: dict) -> tuple[bool, bool]:
    """Check if TreeNode or ListNode are needed."""
    tree = False
    listnode = False
    if is_design_problem(meta):
        all_types = [p["type"] for p in meta.get("constructor", {}).get("params", [])]
        for m in meta.get("methods", []):
            all_types += [p["type"] for p in m.get("params", [])]
            all_types.append(m["return"]["type"])
    else:
        all_types = [p["type"] for p in meta["params"]] + [meta["return"]["type"]]
    for t in all_types:
        if "TreeNode" in t:
            tree = True
        if "ListNode" in t:
            listnode = True
    return tree, listnode


def normalize(s: str) -> str:
    """Normalize output for comparison: strip whitespace, normalize spacing."""
    s = s.strip()
    # Remove spaces after [ and before ], and around commas for consistent comparison
    s = re.sub(r'\s+', ' ', s)
    s = re.sub(r'\[\s+', '[', s)
    s = re.sub(r'\s+\]', ']', s)
    s = re.sub(r'\s*,\s*', ',', s)
    return s


def normalize_any_order(s: str) -> str:
    """Normalize then sort top-level array elements for order-independent comparison."""
    s = normalize(s)
    # Only sort if it's a top-level array like [elem,elem,...]
    if s.startswith("[") and s.endswith("]"):
        inner = s[1:-1]
        # Tokenize top-level elements (respecting nested brackets)
        elems = []
        depth = 0
        cur = ""
        for c in inner:
            if c in "([":
                depth += 1
            elif c in ")]":
                depth -= 1
            if c == "," and depth == 0:
                elems.append(cur.strip())
                cur = ""
            else:
                cur += c
        if cur.strip():
            elems.append(cur.strip())
        return "[" + ",".join(sorted(elems)) + "]"
    return s


def find_solution(problem_number: str) -> str:
    """Find the solution file: check cwd first, then default solve/ dir."""
    cwd_path = os.path.join(os.getcwd(), f"{problem_number}.cpp")
    if os.path.exists(cwd_path):
        return cwd_path
    default_path = os.path.join(SOLVE_DIR, f"{problem_number}.cpp")
    if os.path.exists(default_path):
        return default_path
    return None


def fetch_only(problem_number: str):
    """Fetch and cache test cases without running them."""
    data = load_or_fetch_tests(problem_number)
    title = data["title"]
    test_cases = data["testCases"]
    print(f"\n\033[1m{problem_number}. {title}\033[0m")
    print(f"Fetched {len(test_cases)} example test case(s)")
    for i, tc in enumerate(test_cases):
        print(f"  Test {i + 1}:")
        print(f"    Input:    {tc['input_lines']}")
        if tc.get("expected"):
            print(f"    Expected: {tc['expected']}")


def run_tests(problem_number: str, test_index=None, any_order=False):
    test_file = os.path.join(TESTS_DIR, f"{problem_number}.json")
    if not os.path.exists(test_file):
        fetch_only(problem_number)
        return

    with open(test_file) as f:
        data = json.load(f)
    meta = data["metaData"]
    test_cases = data["testCases"]
    title = data["title"]
    # Auto-detect from cached flag, manual --anyorder overrides
    any_order = any_order or data.get("anyOrder", False)

    solution_path = find_solution(problem_number)
    if not solution_path:
        print(f"Error: {problem_number}.cpp not found in current directory or {SOLVE_DIR}/.")
        print(f"  Run cpgen {problem_number} first, or cd into the folder containing {problem_number}.cpp.")
        sys.exit(1)

    needs_tree, needs_listnode = needs_special_types(meta)
    abs_solution = os.path.abspath(solution_path)
    if is_design_problem(meta):
        runner_code = generate_design_runner_cpp(abs_solution, meta, needs_tree, needs_listnode)
    else:
        runner_code = generate_runner_cpp(abs_solution, meta, needs_tree, needs_listnode)

    # Write runner to temp file
    with tempfile.NamedTemporaryFile(mode='w', suffix='.cpp', dir=SCRIPT_DIR, delete=False) as f:
        runner_path = f.name
        f.write(runner_code)

    binary_path = runner_path.replace('.cpp', '')

    try:
        # Compile — DLOCAL enables dbg(), DCPTEST suppresses solution's main()
        compile_cmd = [
            "g++", "-std=c++17", "-DLOCAL", "-DCPTEST", "-O2",
            "-Wall", "-Wextra", "-Wshadow",
            "-fsanitize=address", "-fsanitize=undefined",
            runner_path, "-o", binary_path,
        ]
        result = subprocess.run(compile_cmd, capture_output=True, text=True)
        if result.returncode != 0:
            print(f"\n\033[31mCompilation failed:\033[0m")
            print(result.stderr)
            return

        # Select which tests to run
        if test_index is not None and test_index <= len(test_cases):
            run_cases = [(test_index - 1, test_cases[test_index - 1])]
        else:
            run_cases = list(enumerate(test_cases))

        print(f"\n\033[1m{problem_number}. {title}\033[0m")
        label = f"test {test_index}" if test_index is not None and len(run_cases) == 1 else f"{len(run_cases)} test(s)"
        order_note = " \033[36m(any order)\033[0m" if any_order else ""
        print(f"Running {label}...{order_note}\n")

        passed = 0
        for i, tc in run_cases:
            stdin_data = "\n".join(tc["input_lines"]) + "\n"
            expected = tc.get("expected")

            try:
                proc = subprocess.run(
                    [binary_path],
                    input=stdin_data,
                    capture_output=True,
                    text=True,
                    timeout=10,
                )
            except subprocess.TimeoutExpired:
                print(f"  Test {i + 1}: \033[33mTLE\033[0m (>10s)")
                print(f"    Input:    {tc['input_lines']}")
                continue

            # Separate debug output (dbg) from sanitizer errors in stderr
            debug_lines = []
            error_lines = []
            if proc.stderr:
                for line in proc.stderr.splitlines():
                    if "ERROR" in line or "runtime error" in line or "SUMMARY" in line:
                        error_lines.append(line)
                    else:
                        debug_lines.append(line)

            if proc.returncode != 0:
                print(f"  Test {i + 1}: \033[31mRTE\033[0m")
                print(f"    Input:    {tc['input_lines']}")
                if error_lines:
                    print(f"    Error:    {error_lines[0].strip()}")
                if debug_lines:
                    for line in debug_lines:
                        print(f"    Debug:    {line}")
                continue

            actual = proc.stdout.strip()

            cmp = normalize_any_order if any_order else normalize

            if expected is None:
                print(f"  Test {i + 1}: \033[33m?\033[0m  (no expected output)")
                print(f"    Input:    {tc['input_lines']}")
                print(f"    Output:   {actual}")
            elif cmp(actual) == cmp(expected):
                passed += 1
                print(f"  Test {i + 1}: \033[32mACCEPTED\033[0m")
                print(f"    Input:    {tc['input_lines']}")
                print(f"    Output:   {actual}")
            else:
                print(f"  Test {i + 1}: \033[31mWRONG ANSWER\033[0m")
                print(f"    Input:    {tc['input_lines']}")
                print(f"    Expected: {expected}")
                print(f"    Output:   {actual}")

            if debug_lines:
                for line in debug_lines:
                    print(f"    Debug:    {line}")

        print(f"\n\033[1mResult: {passed}/{len(run_cases)} passed\033[0m")
        if passed == len(run_cases):
            print("\033[32mAll examples passed!\033[0m")

    finally:
        # Cleanup
        for p in [runner_path, binary_path]:
            if os.path.exists(p):
                os.unlink(p)


def add_test_case(problem_number: str, args: list):
    """Add a custom test case from CLI args.

    Usage (regular):  cptest <num> --add <param1> <param2> ... --expect <expected>
    Usage (design):   cptest <num> --add <methods_array> <args_array> --expect <expected>
    """
    test_file = os.path.join(TESTS_DIR, f"{problem_number}.json")
    if not os.path.exists(test_file):
        load_or_fetch_tests(problem_number)

    with open(test_file) as f:
        data = json.load(f)

    meta = data["metaData"]
    design = is_design_problem(meta)

    # Split args at --expect
    expected = None
    input_args = args
    if "--expect" in args:
        idx = args.index("--expect")
        input_args = args[:idx]
        expected = args[idx + 1] if idx + 1 < len(args) else None

    if design:
        if len(input_args) != 2:
            print(f"Error: design problem expects 2 inputs (methods array, args array), got {len(input_args)}")
            print(f'  Usage:  cptest {problem_number} --add \'["Class","method",...]\' \'[[args],[args],...]\' --expect \'[null,...]\'')
            sys.exit(1)
    else:
        params = meta["params"]
        if len(input_args) != len(params):
            print(f"Error: expected {len(params)} input(s), got {len(input_args)}")
            print(f"  Params: {', '.join(p['name'] + ' (' + p['type'] + ')' for p in params)}")
            print(f"  Usage:  cptest {problem_number} --add {' '.join('<' + p['name'] + '>' for p in params)} --expect <output>")
            sys.exit(1)

    data["testCases"].append({
        "input_lines": input_args,
        "expected": expected,
    })

    with open(test_file, "w") as f:
        json.dump(data, f, indent=2)

    tc_num = len(data["testCases"])
    print(f"Added test {tc_num}: {input_args} -> {expected}")


def main():
    if len(sys.argv) < 2:
        print("Usage: cptest <problem_number> [test_number]")
        print("       cptest 930            (run all tests)")
        print("       cptest 930 1          (run test 1 only)")
        print("       cptest 930 --anyorder (order-independent comparison)")
        print('       cptest 930 --add "[1,2]" "3" --expect "5"')
        print("       cptest 930 --refetch")
        print()
        print("  Looks for <number>.cpp in cwd first, then in solve/.")
        print("  For contest problems, cd into solve/weekly/<n>/ or solve/biweekly/<n>/ first.")
        sys.exit(1)

    problem_number = sys.argv[1]
    any_order = "--anyorder" in sys.argv

    if "--refetch" in sys.argv:
        test_file = os.path.join(TESTS_DIR, f"{problem_number}.json")
        if os.path.exists(test_file):
            os.unlink(test_file)
            print(f"Cleared cached test cases for {problem_number}")

    if "--add" in sys.argv:
        add_idx = sys.argv.index("--add")
        add_args = sys.argv[add_idx + 1:]
        add_test_case(problem_number, add_args)
        return

    # Parse optional test index (second non-flag argument)
    test_index = None
    for arg in sys.argv[2:]:
        if not arg.startswith("-") and arg.isdigit():
            test_index = int(arg)
            break

    run_tests(problem_number, test_index, any_order)


if __name__ == "__main__":
    main()
