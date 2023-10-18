#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <array>
#include <map>
#include <algorithm>
#include <vector>

#define MAX_NODE 18

using namespace std;
using Graph = array<array<int, 9>, 9>;

struct Point {
    int x;
    int y;
    Point(int x, int y) : x(x), y(y) {}
    Point(){}
};

int select_piece(const Graph &graph, const vector<int> &dices,
                 const map<int, Point> &nodes, int piece,
                 const Point &endPoint, const array<int, MAX_NODE> & paceCount) {
    if (piece != 0){
        return piece;
    }

    // find the nearest piece;
    int ret = -1;
    int maxDistance = max(endPoint.x, endPoint.y);
    int step = maxDistance;
    //fprintf(stderr, "max distance: %d!\n", step);
    for (auto && iter : nodes){
        int nodeDistance = max(iter.second.x, iter.second.y);
        int timesPerCycle = paceCount[iter.first];
        if (timesPerCycle == 0){
            continue;
        }
        int curStep = (nodeDistance + 1) / timesPerCycle;
        if (curStep < step){
            step = curStep;
            ret = iter.first;
        }
    }

    //fprintf(stderr, "selected node: %d!\n", ret);
    if (ret == -1){
        fprintf(stderr, "Error on select node!\n");
        exit(1);
    }

    return ret;
}

Point dirToPoint(const Point & curPoint, const int & dir){
    Point nextP = curPoint;
    switch (dir){
        case 0:
            nextP.x -= 1;
            nextP.y -= 1;
            break;
        case 1:
            nextP.x -= 1;
            break;
        case 2:
            nextP.x -= 1;
            nextP.y += 1;
            break;
        case 3:
            nextP.y -= 1;
            break;
        case 4:
            nextP.y += 1;
            break;
        case 5:
            nextP.x += 1;
            nextP.y -= 1;
            break;
        case 6:
            nextP.x += 1;
            break;
        case 7:
            nextP.x += 1;
            nextP.y += 1;
            break;
        default:
            fprintf(stderr, "Don't know how you mean\n");
    }
    return nextP;
}

void play(Graph & graph, array<bool, MAX_NODE> & nodeExist, map<int, Point> & nodes, const int nodeNum, const int dir){
    Point curP = nodes[nodeNum];
    const Point nextP = dirToPoint(curP, dir);

    int graphNode = graph[nextP.x][nextP.y];

    if (graphNode != 0){
        nodeExist[graphNode] = false;
    }

    graph[curP.x][curP.y] = 0;
    graph[nextP.x][nextP.y] = nodeNum;
    nodes[nodeNum].x = nextP.x;
    nodes[nodeNum].y = nextP.y;
}

int main() {

    int n, m;
    Graph graph;
    vector<int> dices;
    map<int, Point> nodes;
    array<int, MAX_NODE> paceCount = {};
    array<bool, MAX_NODE> nodeExist= {};
    scanf("%d %d\n", &n, &m);
    const Point endPoint = {n - 1, m - 1};
    for (int i = 0; i < n; ++i){
        for (int j = 0; j < m; ++j){
            int num;
            scanf("%d", &num);
            //fprintf(stderr, "%d ", num);
            if (num > 0){
                nodes[num] = Point(i, j);
                nodeExist[num] = true;
            }
            graph[i][j] = num;
        }
        scanf("\n");
        //fprintf(stderr, "\n");
    }

    int numOfDices;
    scanf("%d\n", &numOfDices);
    //fprintf(stderr, "%d\n", numOfDices);
    for (int i = 0; i < numOfDices; ++i){
        int dice;
        scanf("%d", &dice);
        dices.push_back(dice);
        paceCount[dice] += 1;
        //fprintf(stderr, "%d ", dice);
    }
    scanf("\n");

    //vector<int> sortedDices = dices;
    //sort(sortedDices.begin(), sortedDices.end());
    
    //fprintf(stderr, "\n");
    //fprintf(stderr, "size: %ld\n", nodes.size());

    int piece;
    scanf("%d\n", &piece);
    //fprintf(stderr, "%d\n", piece);;
    //
    vector<Point> ret;

    const int movePiece = select_piece(graph, dices, nodes, piece, endPoint, paceCount);
    //fprintf(stderr, "The Selected node: %d\n", movePiece);

    while (nodes[movePiece].x != endPoint.x || nodes[movePiece].y != endPoint.y){
        for (auto && node : dices){
            int truePiece = node;

            // find the piece that can be moved
            while (!nodeExist[truePiece]){
                auto it = nodes.find(truePiece);
                if (movePiece > truePiece){
                    ++it;
                }
                else {
                    --it;
                }
                truePiece = it->first;
            }

            if (truePiece == movePiece){
                int xDis = endPoint.x - nodes[truePiece].x;
                int yDis = endPoint.y - nodes[truePiece].y;

                if (xDis > 0 && yDis > 0){
                    play(graph, nodeExist, nodes, truePiece, 7);
                    ret.push_back(Point(truePiece, 7));
                }
                else if (xDis > 0 && yDis == 0){
                    play(graph, nodeExist, nodes, truePiece, 6);
                    ret.push_back(Point(truePiece, 6));
                }
                else if (xDis == 0 && yDis > 0){
                    play(graph, nodeExist, nodes, truePiece, 4);
                    ret.push_back(Point(truePiece, 4));
                }
                if (graph[endPoint.x][endPoint.y] == movePiece){
                    break;
                }
            }
            else{
                const Point & targetPoint = nodes[movePiece];
                int xDis = nodes[truePiece].x;
                int yDis = nodes[truePiece].y;

                if (xDis > 0 && yDis > 0){
                    Point nextP = dirToPoint(nodes[truePiece], 0);
                    if (graph[nextP.x][nextP.y] == movePiece){
                        play(graph, nodeExist, nodes, truePiece, 1);
                        ret.push_back(Point(truePiece, 1));
                    }
                    else {
                        play(graph, nodeExist, nodes, truePiece, 0);
                        ret.push_back(Point(truePiece, 0));
                    }
                }
                else if (xDis > 0 && yDis == 0){
                    Point nextP = dirToPoint(nodes[truePiece], 1);
                    if (graph[nextP.x][nextP.y] == movePiece){
                        play(graph, nodeExist, nodes, truePiece, 4);
                        ret.push_back(Point(truePiece, 1));
                    }
                    else{
                        play(graph, nodeExist, nodes, truePiece, 1);
                        ret.push_back(Point(truePiece, 1));
                    }
                }
                else if (xDis == 0 && yDis > 0){
                    Point nextP = dirToPoint(nodes[truePiece], 3);
                    if (graph[nextP.x][nextP.y] == movePiece){
                        play(graph, nodeExist, nodes, truePiece, 6);
                        ret.push_back(Point(truePiece, 6));
                    }
                    else{
                        play(graph, nodeExist, nodes, truePiece, 3);
                        ret.push_back(Point(truePiece, 3));
                    }
                }
                else if (xDis == 0 && yDis == 0){
                    Point nextP = dirToPoint(nodes[truePiece], 7);
                    if (graph[nextP.x][nextP.y] == movePiece){
                        play(graph, nodeExist, nodes, truePiece, 4);
                        ret.push_back(Point(truePiece, 4));
                    }
                    else{
                        play(graph, nodeExist, nodes, truePiece, 7);
                        ret.push_back(Point(truePiece, 7));
                    }
                }
            }
            //for (int i = 0; i < n; ++i){
                //for (int j = 0; j < n; ++j){
                    //fprintf(stderr, "%d ", graph[i][j]);
                //}
                //fprintf(stderr, "\n");
            //}
            //fprintf(stderr, "selected coord: %d, %d\n", nodes[movePiece].x, nodes[movePiece].y);
            //fprintf(stderr, "end coord: %d, %d\n", endPoint.x, endPoint.y);
            //fprintf(stderr, "======================================\n");
        }

    }


    // Print the received data
    //fprintf(stderr, "Child received data: %.*s\n", (int)num_bytes, buffer);
    //
    printf("%ld\n", ret.size());
    for (auto && point : ret){
        printf(" %d %d\n", point.x, point.y);
    }

    return 0;
}
