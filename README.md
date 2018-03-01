---
lang: ja-jp
title: Pythonを使ってA*探索
tags: Python, A*探索
---
# A\* Search with Python

8パズルをA\*探索を用いて解くプログラムを作る。

## A\*探索のアルゴリズム

- `open` : 展開ノードを格納。ヒープ
- `N` : 注目ノード

1. `open`にルートノードを追加する。
2. `open`が空であれば探索失敗。終了。
3. `open`からノード`N`を取り出す。
4. `N`がゴールノードなら終了。
5. `N`の子ノードを全て展開して`open`に追加。
6. 2.に戻る。

## Effective branching factor(EBF)

分岐数が$N$で、探索木の深さ$d$に対して、

$$
N+1=1+b^*+{b^*}^2+{b^*}^3+\cdots+{b^*}^d
$$

を満たす$b^*$のこと。小さいほどその探索は良い探索である。

### EBF出してみた


|          | heuristic1 | heuristic2 |
| -------- | ---------- | ---------- |
|    1     |            |    3.0     |
|    2     |            |    2.225   |
|    3     |            |    1.954   |
|    4     |            |    1.719   |
|    5     |            |    1.615   |
|    6     |            |    1.542   |
|    7     |            |    1.504   |
|    8     |            |    1.458   |
|    9     |            |    1.432   |
|    10    |            |    1.41    |
|    11    |            |    1.403   |
|    12    |            |    1.391   |
|    13    |            |    1.39    |
|    14    |            |    1.381   |
|    15    |            |    1.383   |
|    16    |            |    1.376   |
|    17    |            |    1.377   |
|    18    |            |    1.371   |
|    19    |            |    1.373   |
|    20    |            |    1.368   |
|    21    |            |    1.37    |
|    22    |            |    1.367   |
|    23    |            |    1.37    |
|    24    |            |    1.367   |
|    25    |            |    1.37    |
|    26    |            |    1.368   |
|    27    |            |    1.372   |
|    28    |            |    1.369   |
|    29    |            |    1.372   |
|    30    |            |    1.371   |
|    31    |            |    1.367   |

## 実装

### 探索プログラム

返り値は`(木の深さ, 展開ノード数, EBF)`としたタプル。
`key`や`close`は、重複した盤面の探索をしないための実装。
盤面は、隣接リストで表現する。
`heapq`モジュールでヒープを利用する。

```python
def aStar(start):
    adjacent = ((1, 3), (0, 2, 4), (1, 5), (0, 4, 6), (1, 3, 5, 7), (2, 4, 8), (3, 7), (4, 6, 8), (5, 7))
    goal = [1,2,3,4,5,6,7,8,0]
    open = []
    close = {}
    open.append(start)
    close[tuple(start.state)] = True
    extracted = 0
    while True:
        if not open:
            print("Search failed.")
            return (999, 0, 0)
        n = heapq.heappop(open)
        if n.state == goal:
            print("Search successed.")
            return (n.depth, extracted, extracted ** (1 / n.depth)) if n.depth else (0, 0, 0)
        else:
            for s in adjacent[n.space]:
                state = copy.deepcopy(n.state)
                state[n.space] = state[s]
                state[s] = 0
                key = tuple(state)
                if not key in close:      
                    heapq.heappush(open, node(state, n.depth + 1, s))
                    close[key] = True
                    extracted += 1
```

### ヒューリスティック関数

```python
# 現状態と最終状態で違う要素の数
def heuristic1(state):
    goal = [1,2,3,4,5,6,7,8,0]
    res = 0
    for i in range(9):
        if state[i]:
            for j in range(9):
                if state[i] != goal[j]:
                    res += 1
    return res

# 同じ要素どうしのマンハッタン距離
def heuristic2(state):
    goal = [1,2,3,4,5,6,7,8,0]    
    res = 0
    for i in range(9):
        if state[i]:
            for j in range(9):
                if state[i] == goal[j]:
                    res += abs(i // 3 - j // 3) + abs(i % 3 - j % 3)
    return res
```

マンハッタン距離の計算では、盤面とリストの対応関係で、リストの要素番号を3で割った商が$x$座標、剰余が$y$座標となることを利用している。


### ノードクラス

```python
class node:
    def __init__(self, state, depth, space):
        self.state = state
        self.depth = depth
        self.space = space
        self.cost = self.depth + heuristic2(state)
    def __eq__(self, other):
        if self is other:
            return True
        elif type(self) == type(other):
            return False
        else:
            return self.cost == other.cost
    def __lt__(self, other):
        return self.cost < other.cost
```

`heapq`モジュールを使うために、ノードクラスはcomparableとなるよう実装した。`__eq__()`と`__lt__()`を用意すればどうもOKらしい。`depth`はこの場合は手数になるので必要。`space`の位置を常に記録していくことで計算の効率化。
