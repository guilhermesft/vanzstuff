
public class WaveFront {
	
	public static final int R = Integer.MAX_VALUE;
	public static final int O = 1;
    public final int xo = 1;
    public final int yo = 0;
    public final int xr = 6;
    public final int yr = 5;
	private  int mMundo[][] = {
			{ 0,  O,  0,  0,  0,  0,  0 },
			{ 0, -1,  0,  0,  0, -1,  0 },
			{ 0,  0,  0, -1,  0,  0, -1 },
			{ 0, -1,  0,  0, -1,  0,  0 },
			{ 0,  0, -1,  0,  0,  0,  0 },
			{ 0,  0,  0,  0,  0, -1,  R }};
	
	public int[][] getMundo(){
        initQuadranteDireitoSuperior(yo,xo);
        initQuadranteDireitoInferior(yo, xo);
        initQuadranteEsquerdoSuperior(yo,xo);
        initQuadranteEsquerdoInferior(yo, xo);
        initEmptyPlaces();
		return mMundo;
		
		
	}

    private void initEmptyPlaces() {
        for(int x = mMundo.length-1; x >= 0 ; x--){
            for(int y = 0; y < mMundo[x].length; y++){
               if(mMundo[x][y] == 0){
                   calculaPosicao(x,y);
               }
            }
        }

    }

    private void initQuadranteEsquerdoInferior(int olinha, int ocoluna) {
        for(int linha = olinha; linha >= 0; linha--){
            for(int coluna = ocoluna; coluna >=0; coluna--){
                if((linha == olinha && coluna == ocoluna) || mMundo[linha][coluna] != 0){
                    continue;
                }
                calculaPosicao(linha,coluna);
            }
        }
    }

    private void initQuadranteEsquerdoSuperior(int olinha, int ocoluna) {
        for(int linha = olinha; linha < this.mMundo.length; linha++){
            for(int coluna = ocoluna; coluna >= 0 ; coluna--){
                if((linha == olinha && coluna == ocoluna) || mMundo[linha][coluna] != 0){
                    continue;
                }
                calculaPosicao(linha,coluna);
            }
        }
    }

    private void initQuadranteDireitoInferior(int olinha, int ocoluna) {
        for(int linha = olinha; linha >= 0; linha--){
            for(int coluna = ocoluna; coluna < mMundo[linha].length; coluna++){
                if((linha == olinha && coluna == ocoluna) || mMundo[linha][coluna] != 0){
                    continue;
                }
                calculaPosicao(linha,coluna);
            }
        }
    }


	private void initQuadranteDireitoSuperior(int olinha, int ocoluna) {
		for(int linha = olinha; linha < this.mMundo.length; linha++){
			for(int coluna = ocoluna; coluna < this.mMundo[linha].length; coluna++){
                if((linha == olinha && coluna == ocoluna) || mMundo[linha][coluna] != 0){
                    continue;
                }
                calculaPosicao(linha,coluna);
			}
		}
		
	}

    private void calculaPosicao(int linha, int coluna){
        //direita
        if(coluna< mMundo[linha].length-1 && mMundo[linha][coluna+1] > 0 ){
            mMundo[linha][coluna] = mMundo[linha][coluna+1] + 1;
            return;
        }
        //abaixo
        if(linha > 0 && mMundo[linha-1][coluna] > 0 && mMundo[linha-1][coluna] != -1){
            mMundo[linha][coluna] = mMundo[linha-1][coluna] + 1;
            return;
        }
        //esquerda
        if(coluna > 0 && mMundo[linha][coluna-1] > 0 ){
            mMundo[linha][coluna] = mMundo[linha][coluna-1] + 1;
            return;
        }
        //acima
        if(linha < mMundo.length-1 && mMundo[linha+1][coluna] >0){
            mMundo[linha][coluna] = mMundo[linha+1][coluna] + 1;
            return;
        }
    }
}
