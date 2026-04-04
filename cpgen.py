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


def generate(problem_number: str):
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

    # Read template
    with open(TEMPLATE_PATH, "r") as f:
        template = f.read()

    # Replace the Solution class placeholder with the fetched one
    cpp = template.replace(
        "class Solution {\n// LeetCode method function\n// void solve() {}\npublic:\n\n};",
        f"// {qid}. {title} [{difficulty}]\n{cpp_snippet}",
    )

    out_path = os.path.join(SOLVE_DIR, f"{qid}.cpp")
    if os.path.exists(out_path):
        print(f"Warning: {out_path} already exists. Overwrite? [y/N] ", end="")
        if input().strip().lower() != "y":
            print("Aborted.")
            return

    os.makedirs(SOLVE_DIR, exist_ok=True)
    with open(out_path, "w") as f:
        f.write(cpp)

    print(f"Created {out_path}")
    print(f"  {qid}. {title} [{difficulty}]")


def main():
    if len(sys.argv) != 2:
        print("Usage: cpgen <problem_number>")
        print("  e.g. cpgen 930")
        sys.exit(1)

    generate(sys.argv[1])


if __name__ == "__main__":
    main()
