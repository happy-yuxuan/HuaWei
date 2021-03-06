//
// Created by Victor Wu on 2019-03-25.
//

#ifndef CODECRAFT_2019_SPARSEGRAPH_H
#define CODECRAFT_2019_SPARSEGRAPH_H

#include <iostream>
#include <vector>
#include <cassert>
#include "entity/Edge.h"
#include "entity/Car.h"

using namespace std;

// 稀疏图 - 邻接表
template<typename Weight>
class SparseGraph{

private:
    int n, m;       // 节点数和边数
    bool directed;  // 是否为有向图
    vector<vector<Edge<Weight> *> > g;   // 图的具体数据
    

public:
    // 车辆的状态
    vector<Car> carList;

    // 构造函数
    SparseGraph( int n , bool directed){
        assert(n >= 0);
        this->n = n;
        this->m = 0;    // 初始化没有任何边
        this->directed = directed;
        // g初始化为n个空的vector, 表示每一个g[i]都为空, 即没有任和边
        g = vector<vector<Edge<Weight> *> >(n+1, vector<Edge<Weight> *>());
    }

    // 析构函数
    ~SparseGraph(){
        for( int i = 1 ; i <= n ; i ++ )
            for( int j = 0 ; j < g[i].size() ; j ++ )
                delete g[i][j];
    }

    int V(){ return n;} // 返回节点个数
    int E(){ return m;} // 返回边的个数

    // 向图中添加一个边, 权值为weight
    void addEdge( int v, int w , Weight weight, int roadId, int road_length, int road_speed, int road_channel){
        assert( v > 0 && v <= n );
        assert( w > 0 && w <= n );

        // 注意, 由于在邻接表的情况, 查找是否有重边需要遍历整个链表
        // 程序允许重边的出现

        g[v].push_back( new Edge<Weight>(v, w, weight, roadId, road_length, road_speed, road_channel) );
        if( v != w && !directed )   // 如果是无向边
            g[w].push_back( new Edge<Weight>(w, v, weight, roadId, road_length, road_speed, road_channel) );
        m ++;
    }

    // 验证图中是否有从v到w的边
    bool hasEdge( int v , int w ){
        assert( v > 0 && v <= n );
        assert( w > 0 && w <= n );
        for( int i = 1 ; i <= g[v].size() ; i ++ )
            if( g[v][i]->other(v) == w )
                return true;
        return false;
    }

    Edge<Weight> * getEdge(int roadId) {
        for (int i = 1; i <= g.size(); i++) {
            for (int j = 0; j <g[i].size(); j++) {
                if (g[i][j]->getRoad() == roadId) {
                    return g[i][j];
                }
            }
        }
        cout << "Don't find roadId" << endl;
        exit(1);
        return NULL;
    }

    // 显示图的信息
    void show(){

        for( int i = 1 ; i <= n ; i ++ ){
            cout << "vertex " << i << ":\t";
            for( int j = 0 ; j < g[i].size() ; j ++ )
                cout << "( to:" << g[i][j]->w()
                << ",wt:" << g[i][j]->wt()
                << ",roadId:" << g[i][j]->getRoad()
                << ",speed:" << g[i][j]->getSpeed()
                << ",length:" << g[i][j]->getLength()
                << ",channel:" << g[i][j]->getChannel()
                << " )\t";
            cout << endl;
        }
    }

    void showCar() {
        for (int i = 0; i < carList.size(); i++) {
            cout << "(第" << carList[i].getId()
            << "辆车，起点为:" << carList[i].getFrom()
            << ",终点为:" << carList[i].getTo()
            << ",速度为:" << carList[i].getSpeed()
            << "," << carList[i].getPlanTime() << ")"
            << endl;
        }
    }

    // 邻边迭代器, 传入一个图和一个顶点,
    // 迭代在这个图中和这个顶点向连的所有边
    class adjIterator{
    private:
        SparseGraph &G; // 图G的引用
        int v;
        int index;

    public:
        // 构造函数
        adjIterator(SparseGraph &graph, int v): G(graph){
            this->v = v;
            this->index = 0;
        }

        ~adjIterator(){}

        // 返回图G中与顶点v相连接的第一个边
        Edge<Weight>* begin(){
            index = 0;
            if( G.g[v].size() )
                return G.g[v][index];
            // 若没有顶点和v相连接, 则返回NULL
            return NULL;
        }

        // 返回图G中与顶点v相连接的下一个边
        Edge<Weight>* next(){
            index += 1;
            if( index < G.g[v].size() )
                return G.g[v][index];
            return NULL;
        }

        // 查看是否已经迭代完了图G中与顶点v相连接的所有顶点
        bool end(){
            return index >= G.g[v].size();
        }
    };
};

#endif //CODECRAFT_2019_SPARSEGRAPH_H
