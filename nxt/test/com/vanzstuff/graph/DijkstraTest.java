package com.vanzstuff.graph;

import junit.framework.Assert;
import junit.framework.TestCase;
import org.junit.Before;
import org.junit.Test;

import java.util.HashSet;
import java.util.Hashtable;
import java.util.Set;

public class DijkstraTest extends TestCase{

    private Set<GraphVertex> mG;
    GraphVertex mS;

    @Before
    public void setUp() throws Exception {
        mS = new TestGraphVertex("s");
        GraphVertex t= new TestGraphVertex("t");
        GraphVertex y = new TestGraphVertex("y");
        GraphVertex x = new TestGraphVertex("x");
        GraphVertex z = new TestGraphVertex("z");

        mS.addAdjacent(y, 5);
        mS.addAdjacent(t, 10);
        y.addAdjacent(t, 3);
        y.addAdjacent(x, 9);
        y.addAdjacent(z, 2);
        t.addAdjacent(y, 2);
        t.addAdjacent(x, 1);
        x.addAdjacent(z, 4);
        z.addAdjacent(mS, 7);
        z.addAdjacent(x, 6);

        mG = new HashSet<GraphVertex>();
        mG.add(mS);
        mG.add(y);
        mG.add(t);
        mG.add(x);
        mG.add(z);
    }

    @Test
    public void testCalculate() throws Exception {
        Dijkstra dijkstra = new Dijkstra();
        Set<GraphVertex> result = dijkstra.calculate(mS, mG);
        int totalVertexCheck = 0;
        for(GraphVertex v : result){
            if("s".equals(v.getId())){
                assertEquals("Custo para o vertice s errado", 0.0, v.getCost());
                assertEquals("Predecessor para o vertice s errado", null, v.getAncestor());
                totalVertexCheck++;
            }
            if("t".equals(v.getId())){
                assertEquals("Custo para o vertice t errado", 8.0, v.getCost());
                assertEquals("Predecessor para o vertice s errado", "y", v.getAncestor().getId());
                totalVertexCheck++;
            }
            if("y".equals(v.getId())){
                assertEquals("Custo para o vertice y errado", 5.0, v.getCost());
                assertEquals("Predecessor para o vertice s errado", "s", v.getAncestor().getId());
                totalVertexCheck++;
            }
            if("x".equals(v.getId())){
                assertEquals("Custo para o vertice x errado", 9.0, v.getCost());
                assertEquals("Predecessor para o vertice s errado", "t", v.getAncestor().getId());
                totalVertexCheck++;
            }
            if("z".equals(v.getId())){
                assertEquals("Custo para o vertice z errado", 7.0, v.getCost());
                assertEquals("Predecessor para o vertice s errado", "y", v.getAncestor().getId());
                totalVertexCheck++;
            }
        }
        assertEquals("Quantidade de vertices processada errada",result.size() , totalVertexCheck);
    }

    private class TestGraphVertex implements GraphVertex{

        private Set<GraphVertex> mAdj;
        private double mCost;
        private GraphVertex mAncestor;
        private Hashtable<GraphVertex, Double> mEdgesCost;
        private String mID;

        public TestGraphVertex(String id){
            mAdj = new HashSet<GraphVertex>();
            mEdgesCost = new Hashtable<GraphVertex, Double>();
            mID = id;
        }

        @Override
        public String getId() {
            return mID;
        }

        @Override
        public Set<GraphVertex> getAdjacents() {
            return mAdj;
        }

        @Override
        public void addAdjacent(GraphVertex v, double edgeCost) {
            mAdj.add(v);
            mEdgesCost.put(v, edgeCost);
        }

        @Override
        public void setCost(double cost) {
            mCost = cost;
        }

        @Override
        public double getCost() {
            return mCost;
        }

        @Override
        public void setAncestor(GraphVertex ancestor) {
            mAncestor = ancestor;
        }

        @Override
        public GraphVertex getAncestor() {
            return mAncestor;
        }

        @Override
        public double getEdgeCost(GraphVertex v) {
            return mEdgesCost.get(v);
        }

    }
}