package com.vanzstuff.graph;

import java.util.Set;

/**
 * Created by vanz on 08/07/14.
 */
public interface GraphVertex {

    public String getId();

    Set<GraphVertex> getAdjacents();
    public void addAdjacent(GraphVertex v, double edgeCost);

    void setCost(double cost);
    double getCost();

    void setAncestor(GraphVertex o);
    GraphVertex getAncestor();

    double getEdgeCost(GraphVertex v);
}



