
# https://leetcode.com/problems/sudoku-solver/


import numpy as np
import itertools
from multiprocessing import Pool
import copy
import time
import random


class Solution:
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



        # TODO: just clean this up and exit properly

        # TODO: also, use python sets below


        while True:

            b = True
            for i, cand in enumerate(candidates):
                if len(cand) == 1:
                    b = False
            if b:
                break

            removes = []
            for i, cand in enumerate(candidates):
                if len(cand) == 1:
                    removes.append(i)
                    inds = self.indices[i]
                    self.board_arr[inds[0], inds[1]] = cand[0]

            for index in sorted(removes, reverse=True):
                del self.indices[index]

            candidates = self.gen_candidates(self.indices, self.board_arr)





        for i, cand in enumerate(candidates):
            if len(cand) == 1:
                print("i: ", i, " cand: ", cand, " ind: ", self.indices[i])




        permutations = itertools.product(*candidates)



        for p in permutations:

            result = self.solve(p)

            if result:

                for c, val in enumerate(result):
                    index = self.indices[c]
                    board[index[0]][index[1]] = val

                print("board: ", board)

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

            # result = self.verify_board(board_arr, self.indices)
            # if not result:
            #     return False

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

            total = np.concatenate((row, col, corner))


            # TODO: Instead of total array use set

            # make list 1-9 (options)
            # turn into set
            # turn total into a set
            # set1.difference(set2)
            # options.difference(total)
            # if options empty, append [] to candidates, and continue


            tot_list = total.tolist()

            candidate_list = []

            for x in range(1, 10):
                if x not in tot_list:
                    candidate_list.append(x)


            if len(candidate_list) == 0:
                print("cl empty: ", index)
                return False

            candidates.append(candidate_list)

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


