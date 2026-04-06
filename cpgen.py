#!/usr/bin/env python3
"""Generate a LeetCode .cpp file by problem number, fetching the signature via API."""

import sys
import os
import json
import urllib.request
import urllib.parse
import re
import textwrap

TEMPLATE_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), "template.cpp")
SOLVE_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "solve")
UPSOLVE_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "upsolve")

LEETCODE_GRAPHQL = "https://leetcode.com/graphql"


def search_problem(number: str) -> str:
    """Search LeetCode by problem number and return the titleSlug."""
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
        headers={
            "Content-Type": "application/json",
            "Referer": "https://leetcode.com",
            "User-Agent": "Mozilla/5.0",
        },
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


def get_problem(slug: str) -> dict:
    """Fetch problem details including C++ code snippet."""
    query = {
        "query": """
        query questionData($titleSlug: String!) {
            question(titleSlug: $titleSlug) {
                questionFrontendId
                title
                titleSlug
                difficulty
                codeSnippets { lang langSlug code }
            }
        }""",
        "variables": {"titleSlug": slug},
    }
    req = urllib.request.Request(
        LEETCODE_GRAPHQL,
        data=json.dumps(query).encode(),
        headers={
            "Content-Type": "application/json",
            "Referer": "https://leetcode.com",
            "User-Agent": "Mozilla/5.0",
        },
    )
    with urllib.request.urlopen(req) as resp:
        data = json.loads(resp.read())
    return data["data"]["question"]


STRUCT_DEFS = {
    "TreeNode": """\
#ifdef LOCAL
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};
#endif""",
    "ListNode": """\
#ifdef LOCAL
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};
#endif""",
}


def detect_structs(cpp_snippet: str) -> list:
    """Detect which struct definitions are needed from the snippet."""
    needed = []
    for name in STRUCT_DEFS:
        if name in cpp_snippet:
            needed.append(name)
    return needed


def strip_definition_comments(cpp_snippet: str) -> str:
    """Remove the commented-out struct definition blocks and usage comments."""
    result = re.sub(r'/\*\*\s*\n(\s*\*.*\n)*\s*\*/\s*\n?', '', cpp_snippet)
    # Collapse multiple blank lines into one
    result = re.sub(r'\n{3,}', '\n\n', result)
    return result.strip()


def extract_method(cpp_snippet: str) -> str:
    """Extract the method signature and body placeholder from LeetCode's C++ snippet."""
    # Remove the outer class Solution { public: ... };
    # Keep only the method(s) inside
    lines = cpp_snippet.strip().splitlines()
    methods = []
    inside = False
    for line in lines:
        stripped = line.strip()
        if stripped in ("class Solution {", "public:", "};"):
            continue
        methods.append(line)

    return "\n".join(methods)


def get_output_dir(contest_type: str = None, contest_number: str = None, upsolve: bool = False) -> str:
    """Get the output directory based on contest type, number, and upsolve flag."""
    base = UPSOLVE_DIR if upsolve else SOLVE_DIR
    if contest_type and contest_number:
        return os.path.join(base, contest_type, contest_number)
    return base


def generate(problem_number: str, contest_type: str = None, contest_number: str = None, upsolve: bool = False):
    print(f"Fetching problem {problem_number}...")
    slug = search_problem(problem_number)
    problem = get_problem(slug)

    title = problem["title"]
    difficulty = problem["difficulty"]
    qid = problem["questionFrontendId"]

    # Find C++ snippet
    cpp_snippet = None
    for snippet in problem["codeSnippets"]:
        if snippet["langSlug"] == "cpp":
            cpp_snippet = snippet["code"]
            break

    if not cpp_snippet:
        print("Error: No C++ snippet found for this problem.")
        sys.exit(1)

    method_body = extract_method(cpp_snippet)

    # Detect needed struct definitions and clean up snippet
    needed_structs = detect_structs(cpp_snippet)
    # Strip /** ... */ comment blocks (struct definitions + usage comments)
    clean_snippet = strip_definition_comments(cpp_snippet)

    # Build struct definitions block
    struct_block = ""
    if needed_structs:
        struct_block = "\n".join(STRUCT_DEFS[s] for s in needed_structs) + "\n\n"

    # Read template
    with open(TEMPLATE_PATH, "r") as f:
        template = f.read()

    # Replace the Solution class placeholder with the fetched one
    cpp = template.replace(
        "class Solution {\n// LeetCode method function\n// void solve() {}\npublic:\n\n};",
        f"// {qid}. {title} [{difficulty}]\n{struct_block}{clean_snippet}",
    )

    out_dir = get_output_dir(contest_type, contest_number, upsolve)
    out_path = os.path.join(out_dir, f"{qid}.cpp")
    if os.path.exists(out_path):
        print(f"Warning: {out_path} already exists. Overwrite? [y/N] ", end="")
        if input().strip().lower() != "y":
            print("Aborted.")
            return

    os.makedirs(out_dir, exist_ok=True)
    with open(out_path, "w") as f:
        f.write(cpp)

    print(f"Created {out_path}")
    print(f"  {qid}. {title} [{difficulty}]")


def parse_args(args: list) -> tuple:
    """Parse flags from args. Returns (contest_type, contest_number, upsolve, remaining_args)."""
    contest_type = None
    contest_number = None
    upsolve = False
    remaining = []
    for arg in args:
        if arg == "--weekly":
            contest_type = "weekly"
        elif arg == "--biweekly":
            contest_type = "biweekly"
        elif arg == "--upsolve":
            upsolve = True
        else:
            remaining.append(arg)

    if contest_type and len(remaining) >= 2:
        contest_number = remaining[1]
        remaining = [remaining[0]] + remaining[2:]
    elif contest_type and len(remaining) < 2:
        print(f"Error: --{contest_type} requires a contest number.")
        print(f"  e.g. cpgen 930 496 --{contest_type}")
        sys.exit(1)

    return contest_type, contest_number, upsolve, remaining


def main():
    if len(sys.argv) < 2:
        print("Usage: cpgen <problem_number>")
        print("       cpgen <problem_number> <contest_number> --weekly")
        print("       cpgen <problem_number> <contest_number> --biweekly")
        print("       cpgen <problem_number> --upsolve")
        print("       cpgen <problem_number> <contest_number> --weekly --upsolve")
        sys.exit(1)

    contest_type, contest_number, upsolve, remaining = parse_args(sys.argv[1:])

    if not remaining:
        print("Error: problem number is required.")
        sys.exit(1)

    generate(remaining[0], contest_type, contest_number, upsolve)


if __name__ == "__main__":
    main()
