from collections import deque
import copy
import random
import sys
import heapq

def heuristic1(state):
    goal = [1,2,3,4,5,6,7,8,0]
    res = 0
    for i in range(9):
        if state[i]:
            for j in range(9):
                if state[i] != goal[j]:
                    res += 1
    return res 

def heuristic2(state):
    goal = [1,2,3,4,5,6,7,8,0]    
    res = 0
    for i in range(9):
        if state[i]:
            for j in range(9):
                if state[i] == goal[j]:
                    res += abs(i // 3 - j // 3) + abs(i % 3 - j % 3)
    return res

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

def printState(node):
    #print("depth : " + str(node.depth))
    for i in range(9):
        if (i + 1) % 3 == 0:
            print(node[i])
        else:
            print(node[i], end = "")
    print("")

def isSolvable(state) -> bool:
    goal = [1,2,3,4,5,6,7,8,0]
    start = list(state)
    cnt = 0
    for i in range(9):
        if not start[i]:
            dist = abs(2 - i // 3) + abs(2 - i % 3)
            break
    #print(dist)
    #printState(start)    
    for i in range(9):
        for j in range(9):
            if start[j] == goal[i]:
                start[j], start[i] = start[i], start[j]
                #printState(start)
                if not i == j:
                    cnt += 1
        if start == goal:
            break
    if dist % 2 == cnt % 2:
        return True
    else:
        return False


def bfs(start):
    adjacent = ((1, 3), (0, 2, 4), (1, 5), (0, 4, 6), (1, 3, 5, 7), (2, 4, 8), (3, 7), (4, 6, 8), (5, 7))    
    goal = [1,2,3,4,5,6,7,8,0]
    open = deque([])
    close = {}
    open.append(start)
    close[tuple(start.state)] = True
    extracted = 0
    #print('NOW SEARCHING...')
    while True:
        if not open:
            print("SEARCH FAILED")
            exit()
        n = open.popleft()
        #print("Depth : " + str(n.depth))        
        #print(extracted)
        #printState(n)
        if n.state == goal:
            #print("SEARCH SUCCESSED")
            print("Extracted " + str(extracted) + " nodes.")
            print("Depth : " + str(n.depth))
            exit()
        else:
            for op in adjacent[n.space]:
                state = copy.deepcopy(n.state)
                state[n.space] = state[op]
                state[op] = 0
                key = tuple(state)
                if not key in close:
                    tmp = node(state, n.depth + 1, op)           
                    open.append(tmp)
                    close[key] = True
                    extracted += 1
                    #print(extracted)




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
            return (n.depth, extracted, float(format(extracted ** (1 / n.depth), '.3f'))) if n.depth else (0, 0, 0)
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
                        

def initialStateGen():
    start = [1,2,3,4,5,6,7,8,0] 
    start_list = [] 
    seen = {}
    i = 362880
    while i:
        sys.stdout.write("\rGenerating starts...%06d" % i)
        sys.stdout.flush()
        shuffled = list(start)
        random.shuffle(shuffled)
        key = tuple(shuffled)        
        if isSolvable(shuffled):
            if not key in seen:
                start_list.append(shuffled)
                seen[key] = True
                i -= 1
        else:
            if not key in seen:
                seen[key] = True
                i -= 1
    print("Generating start completed.")
    f = open('list.txt', 'w')
    f.write(str(start_list) + "\n")
    f.close()
    #successed
    #start = [2,8,3,1,6,4,7,0,5]
    #goal = [1,2,3,8,0,4,7,6,5]
    #start = [8,4,5,3,2,0,6,7,1]
    #goal = [1,2,3,4,5,6,7,8,0]

    #31(max) step
    #start = [8,6,7,2,5,4,3,0,1]
    #start = [6,4,7,8,5,0,3,2,1]
    #goal = [1,2,3,4,5,6,7,8,0]

def search():
    data = []
    start_list = []
    sll = len(start_list)
    if not sll == 181440:
        print('invalid starts : ' + str(sll))
        exit()
    for start in start_list:
        sys.stdout.write("\rSearching...%06d" % sll)
        sys.stdout.flush()
        for i in range(9):
            if start[i] == 0:
                space = i
        data.append(aStar(node(start, 0, space)))
        sll -= 1

    f = open('data1.txt', 'w')
    f.write(str(data) + "\n")
    f.close()

def calcAveOfEBF():
    ebf_list = []
    data = []
    for i in range(32):
        sys.stdout.write("\r Calculating EBF. %d" % i)
        sys.stdout.flush()
        ebf = 0
        num = 0
        for d in data:
            if d[0] == i :
                num += 1
                if d[0] == 0:
                    ebf += 0
                else:
                    ebf += d[2]
        if num:
            ebf_list.append(float(format(ebf / num, '.3f')))
    
    print(ebf_list)
    f = open('ebf1.txt', 'w')
    f.write(str(ebf_list) + "\n")
    f.close()

def solve():
    #goal = [1,2,3,4,5,6,7,8,0]

    #start = [8,4,5,3,2,0,6,7,1]
    #31(max) step
    #start = [8,6,7,2,5,4,3,0,1]
    start = [6,4,7,8,5,0,3,2,1]

    for i in range(9):
        if not start[i]:
            space = i
            break

    print('(Depth, Branches, EBF) = ' + str(aStar(node(start, 0, space))))

if __name__ == '__main__':
    solve()
