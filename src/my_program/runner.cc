#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <array>
#include <unordered_map>
#include <vector>

using namespace std;
using Graph = array<array<int, 9>, 9>;

struct Point {
    int x;
    int y;
    Point(int x, int y) : x(x), y(y) {}
    Point(){}
};

int select_piece(const Graph &graph, const vector<int> &dices,
                 const unordered_map<int, Point> &nodes, int piece,
                 const Point &endPoint) {
    if (piece != 0){
        return piece;
    }
    // find the nearest piece;
}

int main() {

    int n, m;
    Graph graph;
    vector<int> dices;
    unordered_map<int, Point> nodes;
    array<int, 18> paceCount = {};
    scanf("%d %d\n", &n, &m);
    Point endPoint = {n - 1, m - 1};
    for (int i = 0; i < n; ++i){
        for (int j = 0; j < m; ++j){
            int num;
            scanf("%d", &num);
            //fprintf(stderr, "%d ", num);
            if (num > 0){
                nodes[num] = Point(i, j);
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
    //fprintf(stderr, "\n");

    int piece;
    scanf("%d\n", &piece);
    //fprintf(stderr, "%d\n", piece);;

    int movePiece = select_piece(graph, dices, nodes, piece, endPoint);


    // Print the received data
    //fprintf(stderr, "Child received data: %.*s\n", (int)num_bytes, buffer);
    //
    printf("6\n");
    printf(" 3 6\n");
    printf(" 5 4\n");
    printf(" 2 7\n");
    printf(" 5 4\n");
    printf(" 2 1\n");
    printf(" 5 4\n");

    return 0;
}
