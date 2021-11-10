
# Given a string s, return the longest palindromic substring in s.

# Input: s = "babad"
# Output: "bab"
# Note: "aba" is also a valid answer.

# Input: s = "cbbd"
# Output: "bb"


s = "babada"

output = []
for char in s:
    output.append(char)

output.reverse()

s_i = "".join(output)

max_len = 0

for i in range(len(s)):
    for j in range(len(s)):
        if j > i:
            try:
                out = s_i.index(s[i:j])
                print(s[i:j])
                max_len = max(max_len, len(s[i:j]))
            except:
                pass

print(max_len)