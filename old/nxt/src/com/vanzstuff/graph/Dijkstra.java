package com.vanzstuff.graph;

import java.util.HashSet;
import java.util.List;
import java.util.Set;

/**
 * Created by vanz on 08/07/14.
 */
public class Dijkstra extends MinPathAlgoritm{

    public  Set<GraphVertex> calculate(GraphVertex initVertex, Set<GraphVertex> gVertexes){
        init(initVertex,gVertexes);
        Set<GraphVertex> vertexes = new HashSet<GraphVertex>(gVertexes);
        Set<GraphVertex> s = new HashSet<GraphVertex>();
        while(!vertexes.isEmpty()){
            GraphVertex u = extractMin(vertexes);
            s.add(u);
            for(GraphVertex v : u.getAdjacents()){
                relax(u, v);
            }
        }
        return s;
    }


    private GraphVertex extractMin(Set<GraphVertex> vertexes){
        GraphVertex v = null;
        for(GraphVertex gv : vertexes){
            if( v == null){ // primeiro vertice
                v = gv;
            }else{
                if(gv.getCost() < v.getCost()){
                    v = gv;
                }
            }
        }
        vertexes.remove(v);
        return v;
    }


}
