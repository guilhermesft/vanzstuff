package com.vanzstuff.graph;

import java.util.Set;

/**
 * Created by vanz on 08/07/14.
 */
public class MinPathAlgoritm {

    /**
     * Executa a inicialização dos vertices para execução do algoritmo
     * @param s ponto de partida
     * @param g conjunto de vertices do grafo
     */
    protected void init(GraphVertex s, Set<GraphVertex> g){
        for( GraphVertex gv : g){
            gv.setCost(Double.MAX_VALUE);
            gv.setAncestor(null);
        }
        s.setCost(0.0);
    }

    protected void relax(GraphVertex u, GraphVertex v){
        if (v.getCost() > ( u.getCost() + u.getEdgeCost(v) )){
            v.setCost(( u.getCost() + u.getEdgeCost(v) ));
            v.setAncestor(u);
        }
    }

}
