
# https://leetcode.com/problems/sudoku-solver/


import numpy as np
import itertools
from multiprocessing import Pool
import copy
import time
import random


class Solution:

    # def check

    def solveSudoku(self, board):
        """
        Do not return anything, modify board in-place instead.
        """

        self.indices = []

        for i in range(9):
            for j in range(9):
                if board[i][j] == ".":
                    self.indices.append([i, j])
                    board[i][j] = "0"

        self.board_arr = np.array(board).astype(int)

        # create list of candidates
        candidates = self.gen_candidates(self.indices, self.board_arr)

        while True:                        
            removes = []
            for i, cand in enumerate(candidates):
                if len(cand) == 1:
                    removes.append(i)
                    inds = self.indices[i]
                    self.board_arr[inds[0], inds[1]] = cand[0]

            if len(removes) == 0:
                break

            for index in sorted(removes, reverse=True):
                del self.indices[index]

            candidates = self.gen_candidates(self.indices, self.board_arr)


        if len(candidates) == 0:
            verified = True

            for i in range(0, 9):
                for j in range(0, 9):

                    open_set = set([i for i in range(1, 10)])

                    row, col, corner = self.get_arrs([i, j], self.board_arr)

                    final_row = len(open_set - set(row))
                    final_col = len(open_set - set(col))
                    final_cor = len(open_set - set(corner))

                    if final_row > 0 or final_col > 0 or final_cor > 0:
                        verified = False

            if verified:
                print("verified: ", verified)
                return

        permutations = itertools.product(*candidates)

        for p in permutations:

            result = self.solve(p)

            if result:

                for c, val in enumerate(result):
                    index = self.indices[c]
                    board[index[0]][index[1]] = val

                ba = np.array(board).astype(int)

                verified = self.verify_board(ba, self.indices)

                print("verified: ", verified)


                return


    def solve(self, combo):

        print("combo: ", combo)

        board_arr = copy.deepcopy(self.board_arr)

        result = False

        for c, val in enumerate(combo):
            index = self.indices[c]
            board_arr[index[0], index[1]] = val

            p = c + 1
            if p >= len(combo):
                return combo

            candidates = self.gen_candidates(self.indices[p:], board_arr)
            if not candidates:
                return False

        if result:
            return combo

        return False


    def verify_board(self, board_arr, indices):
        for index in indices:

            row, col, corner = self.get_arrs(index, board_arr)

            a = self.detect_duplicates(row)
            b = self.detect_duplicates(col)
            c = self.detect_duplicates(corner)

        if a or b or c:
            return False

        return True


    def detect_duplicates(self, arr):
        s = np.sort(arr, axis=None)
        out = s[:-1][s[1:] == s[:-1]]

        if out.size > 0 and out[-1] != 0:
            return True

        return False


    def get_arrs(self, index, board_arr):
        i = index[0]
        j = index[1]

        x = int(i / 3) * 3
        y = int(j / 3) * 3

        # generate list of options
        row = board_arr[i]
        col = board_arr[:, j]
        corner = board_arr[x:x + 3, y:y + 3].flatten()

        return row, col, corner


    def gen_candidates(self, indices, board_arr):

        candidates = []

        for index in indices:

            row, col, corner = self.get_arrs(index, board_arr)

            filled_set = set(row) | set(col) | set(corner)

            initial_set = set([x for x in range(1, 10)])

            final_set = initial_set - filled_set

            if len(final_set) == 0:
                return False

            candidates.append(list(final_set))

        return candidates



board = [["5","3",".",".","7",".",".",".","."],["6",".",".","1","9","5",".",".","."],[".","9","8",".",".",".",".","6","."],["8",".",".",".","6",".",".",".","3"],["4",".",".","8",".","3",".",".","1"],["7",".",".",".","2",".",".",".","6"],[".","6",".",".",".",".","2","8","."],[".",".",".","4","1","9",".",".","5"],[".",".",".",".","8",".",".","7","9"]]

# board = [
# ["3","6","1","4","8","7","2","5","9"],
# ["8","5","9","2","6","3","1","4","7"],
# ["7","4","2","5","9","1","8","3","6"],
# ["4","2","8","9","1","6","5","7","3"],
# ["9","7","5","3","2","4","6","8","1"],
# ["1","3","6","7","5","8","4","9","2"],
# ["5","9","3","6","4","2","7","1","8"],
# ["6","1","4","8","7","9","3","2","5"],
# ["2","8","7","1","3","5","9","6","4"]]

# board = [
# ["3","6","1","4","8","7","2","5","9"],
# ["8","5","9","2","6","3","1","4","7"],
# ["7","4","2","5","9","1","8","3","6"],
# ["4","2","8","9","1","6","5","7","3"],
# ["9","7","5","3","2","4","6","8","1"],
# ["1","3","6","7","5","8","4","9","2"],
# ["5","9","3","6","4","2","7","1","8"],
# ["6","1",".",".",".",".",".",".","."],
# [".",".",".",".",".",".",".",".","."]]


sol = Solution()
sol.solveSudoku(board)


