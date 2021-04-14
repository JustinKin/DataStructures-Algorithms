#ifndef __IGRAPH_H__
#define __IGRAPH_H__

#include <vector>

using namespace std;

//Graph 图
template <typename Tv, typename Te> class Graph
{
private:
    void reset()  //所有顶点 边的辅助信息复位
    {
        for (int i = 0; i < n; ++i)  //顶点
        {
            status(i) = UNDISCOVERED;
            dTime(i) = fTime(i) = -1;
            parens(i) = -1;
            priority(i) = INT_MAX;
            for(int j = 0; j < n; ++j)  //边
                if(exists(i, j)) type(i, j) = UNDISCOVERED;
        }
    }

public:
};


// Vertex 顶点
using VStatus = enum { UNDISCOVERED, DISCOVERED, VISITED };
template <typename Tv> struct Vertex  //未严格封装
{
    Tv data;
    int inDegree, outDegree;  // 出入度
    VStatus status;  // 如上三种状态
    int dTime, fTime; // 时间标签 被发现 访问完毕
    int parent;  // 在遍历树中的父节点
    int priority;  // 在遍历树中的优先级 最短通路 极短跨边等
    Vertex(Tv const & d) : // 构造新顶点
        data(d), inDegree(0), outDegree(0), status(UNDISCOVERED),
        dTime(-1), fTime(-1), parent(-1), priority(INT_MAX) {}
};


// Edge 边
using Etype = enum{ UNDISCOVERED, TREE, CROSS, FORWARD, BACKWARD };
template <typename Te> struct Edge  //未严格封装
{
    Te data;
    int weight;
    Etype type;  // 在遍历树中所属的类型
    Edge(Te const &d, int w) : // 构造新边
        data(d), weight(w), type(UNDISCOVERED) {}
};


// GraphMatrix 基于邻接矩阵实现的图结构 一种形式
template <typename Tv, typename Te> class
    GraphMatrix : public Graph<Tv, Te>
    {
    private:
        vector<Vertex<Tv>> v; // 顶点集 规模 n
        vector<vector<Edge<Te>* >> E; // 边集


    public:
        GraphMatrix() { n = e = 0; }
        ~GraphMatrix()
        {
            for(int j = 0; j < n; ++j)
                for(int k = 0; k < n; ++k)
                    delete E[j][k];  // 清楚所有边记录
        }

        // 顶点读写
        Tv& vertex(int i) { return v[i].data; }
        int inDegree(int i) { return v[i].inDegree; }
        int outDegree(int i) { return v[i].outDegree; }
        VStatus& status(int i) { return v[i].status; }
        int& dTime(int i) { return v[i].dTime; }
        int& fTime(int i) { return v[i].fTime; }
        int& parent(int i) { return v[i].parent; }
        int& priority(int i) { return v[i].priority; }

        // 边的读写
        bool exists(int i, int j) // 判断边(i, j)是否存在 短路求值
        {
            return (0 <= i) && (i < n) && (0 <= j) && (j < n)
                && E[i][j] != NULL;
        }  // 以下假定 exists(i, j) = true
        Te& edge(int i, int j) { return E[i][j] -> data; }  // O(1)
        Etype& type(int i, int j) { return E[i][j] -> type; }  // O(1)
        int& weight(int i, int j) { return E[i][j] -> weight; }  // O(1)

        // 邻点的枚举
        int firstNbr(int i) { return nextNbr(i, n) ;}  // 假想哨兵
        int nextNbr(int i, int j)
        {
            while ((-1 < j) && !exists(i, --j));  // 逆序查找
                return j;
        }  // O(n) 改用邻接表可以提高至 O(1 + outDegree)
    };







#endif //__IGRAPH_H__