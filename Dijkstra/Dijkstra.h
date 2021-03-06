//
// Created by Victor Wu on 2019-03-25.
//

#ifndef CODECRAFT_2019_DIJKSTRA_H
#define CODECRAFT_2019_DIJKSTRA_H

#include <iostream>
#include <vector>
#include <stack>
#include "../entity/Edge.h"
#include "IndexMinHeap.h"
#include "../entity/Car.h"

using namespace std;

// Dijkstra算法求最短路径 1 -> 0
template<typename Graph, typename Weight>
class Dijkstra{

private:
    Graph &G;                   // 图的引用
    int s;                      // 起始点
    Weight *distTo;             // distTo[i]存储从起始点s到i的最短路径长度
    bool *marked;               // 标记数组, 在算法运行过程中标记节点i是否被访问
    vector<Edge<Weight>*> from; // from[i]记录最短路径中, 到达i点的边是哪一条
    // 可以用来恢复整个最短路径

public:
    // 构造函数, 使用Dijkstra算法求最短路径
    Dijkstra(Graph &graph, int s):G(graph){

        // 算法初始化
        assert( s > 0 && s <= G.V() );
        this->s = s;
        distTo = new Weight[G.V()];
        marked = new bool[G.V()];
        for( int i = 1 ; i <= G.V() ; i ++ ){
            distTo[i-1] = Weight();
            marked[i-1] = false;
            from.push_back(NULL);
        }

        // 使用索引堆记录当前找到的到达每个顶点的最短距离
        IndexMinHeap<Weight> ipq(G.V());

        // 对于其实点s进行初始化
        distTo[s] = Weight();   // 自己到自己一定是0， 但是模版类用的是自己的默认构造函数
        marked[s] = true;
        from[s] = new Edge<Weight>(s, s, Weight());
        ipq.insert(s, distTo[s] );
        
        while( !ipq.isEmpty() ){
            int v = ipq.extractMinIndex();

            // distTo[v]就是s到v的最短距离
            marked[v] = true;

            // 对v的所有相邻节点进行更新 ！！松弛操作
            typename Graph::adjIterator adj(G, v);
            for( Edge<Weight>* e = adj.begin() ; !adj.end() ; e = adj.next() ){
                int w = e->other(v);
                // 如果从s点到w点的最短路径还没有找到
                if( !marked[w] ){
                    // 如果w点以前没有访问过,
                    // 或者访问过, 但是通过当前的v点到w点距离更短, 则进行更新
                    if( from[w] == NULL || distTo[v] + e->wt() < distTo[w] ){
                        distTo[w] = distTo[v] + e->wt();
                        from[w] = e;
                        if( ipq.contain(w) )
                            ipq.change(w, distTo[w] );
                        else
                            ipq.insert(w, distTo[w] );
                    }
                }
            }
        }
    }

    // 析构函数
    ~Dijkstra(){
        delete[] distTo;
        delete[] marked;
        delete from[0];
    }

    // 返回从s点到w点的最短路径长度
    Weight shortestPathTo( int w ){
       assert( w >= 0 && w < G.V() );
       assert( hasPathTo(w) );
        return distTo[w];
    }

    // 判断从s点到w点是否联通
    bool hasPathTo( int w ){
       assert( w >= 0 && w < G.V() );
        return marked[w];
    }

    // 寻找从s到w的最短路径, 将整个路径经过的边存放在vec中
    void shortestPath( int w, vector<Edge<Weight>> &vec ){

        assert( w >= 0 && w < G.V() );
        assert( hasPathTo(w) );

        // 通过from数组逆向查找到从s到w的路径, 存放到栈中
        stack<Edge<Weight>*> s;
        Edge<Weight> *e = from[w];
        while( e->v() != this->s ){
            s.push(e);
            e = from[e->v()];
        }
        s.push(e);

        // 从栈中依次取出元素, 获得顺序的从s到w的路径
        while( !s.empty() ){
            e = s.top();
            vec.push_back( *e );
            s.pop();
        }
    }

    // 打印出从s点到w点的路径
    void showPath(int w, Car &car){

        assert( w >= 0 && w < G.V() );
        assert( hasPathTo(w) );

        vector<Edge<Weight>> vec;
        shortestPath(w, vec);
        for( int i = 0; i < vec.size() ; i ++ ){
            car.roadList.push_back(vec[i].getRoad());   // 把车的行驶路线放到Car类中对应的路线中
//            打印去到的节点
//            res.push_back(vec[i].v());
//            cout<<vec[i].v()<<" -> ";
//            if( i == vec.size()-1 ) {
//                cout<<vec[i].w()<<endl;
//                res.push_back(vec[i].w());
//            }

        }
    }
};

#endif //CODECRAFT_2019_DIJKSTRA_H
