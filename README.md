sparseMEM
=========

[![Build Status](https://travis-ci.org/reisub/sparseMEM.svg?branch=master)](https://travis-ci.org/reisub/sparseMEM)

An implementation of an algorithm for finding maximal exact matches in large sequence datasets using sparse suffix arrays

## Terminology

`S` - a string of `n` characters stored in an array

`$` - the termination character of the string, the unique lexicographically smallest character in the alphabet which contains the possible characters in `S`

`suf(S, i)` - a suffix of the string `S` starting with the character `S[i]` all the way to the termination character `$`

`SA` - suffix array, a sorted array of all suffixes of a string

### S-type an L-type

A suffix `suf(S, i)` is said to be S-type or L-type if `suf(S, i) < suf(S, i+1)` or `suf(S, i) > suf(S, i+1)`, respectively.

The last suffix `suf(S, n-1)`, consisting of only a single termination character `$` is defined as S-type.

A character `S[i]` is S-type or L-type if the suffix `suf(S, i)` is S-type or L-type, respectively.

### Buckets and sub-buckets

a `bucket` is a sub-array of the `SA` for all the suffixes starting with the same character. It can be further split into two sub-buckets with respect to the types of the suffixes inside: the L- and S-type buckets, where the L- type bucket is on the left of the S-type bucket.

### LMS

`LMS` - left-most S-type

A character `S[i]` i called LMS if `S[i]` i S-type and `S[i+i]` is L-type. A suffix `suf(S, i)` is called LMS is `S[i]` is a LMS character.

A LMS-substring is a substring `S[i..j]` with both `S[i]` and `S[j]` being LMS characters and there is no other LMS character in the substring. The termination character `$` is also a LMS-substring.

### Graphical example

S - `cccagaaaactaccacctccggccagta$`

|        | index | Type | suffix                        |
| ------ | ----- | ---- | ----------------------------- |
| [0]    | 28    | S    | $                             |
| [1]    | 27    | L    | a$                            |
| [2]    | 5     | S    | aaaactaccacctccggccagta$      |
| [3]    | 6     | S    | aaactaccacctccggccagta$       |
| [4]    | 7     | S    | aactaccacctccggccagta$        |
| [5]    | 11    | S    | accacctccggccagta$            |
| [6]    | 14    | S    | acctccggccagta$               |
| [7]    | 8     | S    | actaccacctccggccagta$         |
| [8]    | 3     | S    | agaaaactaccacctccggccagta$    |
| [9]    | 24    | S    | agta$                         |
| [10]   | 13    | L    | cacctccggccagta$              |
| [11]   | 2     | L    | cagaaaactaccacctccggccagta$   |
| [12]   | 23    | L    | cagta$                        |
| [13]   | 12    | L    | ccacctccggccagta$             |
| [14]   | 1     | L    | ccagaaaactaccacctccggccagta$  |
| [15]   | 22    | L    | ccagta$                       |
| [16]   | 0     | L    | cccagaaaactaccacctccggccagta$ |
| [17]   | 18    | S    | ccggccagta$                   |
| [18]   | 15    | S    | cctccggccagta$                |
| [19]   | 19    | S    | cggccagta$                    |
| [20]   | 9     | S    | ctaccacctccggccagta$          |
| [21]   | 16    | S    | ctccggccagta$                 |
| [22]   | 4     | L    | gaaaactaccacctccggccagta$     |
| [23]   | 21    | L    | gccagta$                      |
| [24]   | 20    | L    | ggccagta$                     |
| [25]   | 25    | S    | gta$                          |
| [26]   | 26    | L    | ta$                           |
| [27]   | 10    | L    | taccacctccggccagta$           |
| [28]   | 17    | L    | tccggccagta$                  |
