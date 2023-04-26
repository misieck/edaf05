import sys
import os
import copy
import heapq

nodes = []
visited = []
T = []
nvisited = 0


path = os.path.abspath( "./data/sample/1.in"  )
# replace infile with sys.stdin when done-ish
infile = open(path);


(N, M) = list(map(int, infile.readline().split()))


#array comprehension:  nodes is an array, each element correspond to an element in range(size[0]) i.e. 0..n;
#in this case each element is just [] (empty list), but elements of the "in" list can be used to construct the "comprehended" elements 
nodes = [[] for _ in  range(N)]
visited = [False] * N

for line in infile:
    # map(func, list) - apply func on each element in list, return an iterable of the resulting list
    # because map returns iterable, we need to make a list out of it
    # lambda defines a no-name function, here the argument is x
    #numbers = list(map( lambda x: int(x)-1, line.split()))

    #same as above but with array comprehension
    numbers = [int(x) - 1 for x in line.split()] 

    print ( "readin in ", numbers)
    nodes[numbers[0]].append((numbers[2], numbers[1]))
    nodes[numbers[1]].append((numbers[2], numbers[0]))

print (nodes)

def extendFrontier(newnode, heap):
    for edge in newnode:
        heapq.heappush(heap, edge)

def getNode( n):
    return n[1]
        
Q = copy.deepcopy(nodes[1:])


T.append((0,0))
visited[0] = True
nvisited += 1
heap = []
extendFrontier( nodes[0], heap )

#while nvisited < N:
newedge = heapq.heappop(heap)
T.append(newedge)
# todo: check if edge leads to visited
extendFrontier( nodes[getNode(newedge)], heap)
visited[ getNode(newedge) ] = True
nvisited += 1





print(heap)



