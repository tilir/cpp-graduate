"""Black-box tests; only the Python standard library is required."""

from pathlib import Path
import random
import subprocess
import sys
import unittest


BINARY = Path(sys.argv.pop(1) if len(sys.argv) > 1 else "./lru").resolve()


def reference_hits(capacity, values):
    cache = []
    hits = 0
    for value in values:
        if value in cache:
            hits += 1
            cache.remove(value)
        cache.insert(0, value)
        del cache[capacity:]
    return hits


class LRUTests(unittest.TestCase):
    def run_lru(self, data):
        return subprocess.run(
            [str(BINARY)], input=data, text=True, capture_output=True, timeout=5
        )

    def check_hits(self, capacity, values, expected):
        data = f"{capacity} {len(values)}\n" + " ".join(map(str, values))
        result = self.run_lru(data)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(result.stderr, "")
        self.assertEqual(result.stdout, f"{expected}\n", data)

    def test_examples_and_boundaries(self):
        cases = [
            (2, [1, 2, 1, 2, 1, 2], 4),
            (0, [], 0),
            (0, [1, 1, 1], 0),
            (5, [], 0),
            (1, [7], 0),
            (1, [7, 7, 7, 7], 3),
            (1, [1, 2, 1, 2], 0),
            (10, [1, 2, 3, 1, 2, 3], 3),
            (2, [1, 2, 3, 4, 5], 0),
            (2, [1, 2, 3, 1, 2, 3], 0),
            (2, [1, 2, 1, 3, 1, 2], 2),  # A hit must update recency.
            (3, [1, 2, 3, 2, 4, 2, 3, 1], 3),
            (3, [0, -1, -2, 0, -1, -2], 3),
            (2, [-(2**63), 2**63 - 1, -(2**63), 2**63 - 1], 2),
            (2**63 - 1, [1, 1], 1),
        ]
        for capacity, values, expected in cases:
            with self.subTest(capacity=capacity, values=values):
                self.check_hits(capacity, values, expected)

    def test_whitespace(self):
        result = self.run_lru(" \t2\n6\n1\t2\n1 2\n1 2\n")
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(result.stderr, "")
        self.assertEqual(result.stdout, "4\n")

    def test_invalid_input(self):
        for data in ["", "2", "bad", "-1 0", "1 -1", "2 3 1 2",
                     "2 1 bad", "0 2 1"]:
            with self.subTest(data=data):
                result = self.run_lru(data)
                self.assertNotEqual(result.returncode, 0)
                self.assertEqual(result.stdout, "")
                self.assertTrue(result.stderr)

    def test_random_against_reference(self):
        rng = random.Random(42)
        for index in range(300):
            capacity = rng.randrange(21)
            values = [rng.randrange(-25, 26) for _ in range(rng.randrange(301))]
            with self.subTest(index=index, capacity=capacity):
                self.check_hits(capacity, values, reference_hits(capacity, values))

    def test_long_sequence(self):
        values = list(range(100)) * 1000
        self.check_hits(100, values, len(values) - 100)
        self.check_hits(99, values, 0)


if __name__ == "__main__":
    unittest.main()
