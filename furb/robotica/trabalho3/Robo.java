/**
 * Created by vanz on 19/07/14.
 */
public class Robo {

    private static final int SUL = 1;
    private static final int NORTE = 2;
    private static final int OESTE= 3;
    private static final int LESTE = 4;
    private static final int MOVE_ROTATE = 23*25;
    private static final int ANGLE_ROTATE = 90;

    private int mMundo[][];
    private int mX;
    private int mY;
    private int mOrientacaoAtual;
  //  static NXTRegulatedMotor mMotorDireita = Motor.A;
  //  static NXTRegulatedMotor mMotorEsquerda = Motor.B;


    public Robo(WaveFront waveFront){
        mMundo = waveFront.getMundo();
        mX = waveFront.xr;
        mY = waveFront.yr;
        mOrientacaoAtual = NORTE;
    }

    public void start(){

       while(mMundo[mY][mX] != WaveFront.O){
           printMundo();
           int direcao = checarMenorVizinho();
           andar(direcao);
        }
        System.out.println("Robo chegou ao objetivo = ( " + mY + ", " + mX + ")");
    }

    private void andar(int direcao) {
        if(direcao == SUL){
           andarSul();
        }else if(direcao == NORTE){
            andarNorte();
        }else if(direcao == LESTE){
            andarLeste();
        }else if(direcao == OESTE){
            andarOeste();
        }
    }

    private void andarOeste() {
        mX -= 1;
        if(mOrientacaoAtual == NORTE){
            virarEsquerda();
            avanca();
        }else if(mOrientacaoAtual == SUL){
            virarDireita();
            avanca();
        }else if( mOrientacaoAtual == LESTE){
            virarDireita();
            virarDireita();
            avanca();
        }else if( mOrientacaoAtual == OESTE){
            avanca();
        }
        mOrientacaoAtual = OESTE;
    }



    private void andarLeste() {
        mX += 1;
        if(mOrientacaoAtual == NORTE){
            virarDireita();
            avanca();
        }else if(mOrientacaoAtual == SUL){
            virarEsquerda();
            avanca();
        }else if( mOrientacaoAtual == LESTE){
            avanca();
        }else if( mOrientacaoAtual == OESTE){
            virarDireita();
            virarDireita();
            avanca();
        }
        mOrientacaoAtual = LESTE;
    }

    private void andarNorte() {
        mY += 1;
        if(mOrientacaoAtual == NORTE){
            avanca();
        }else if(mOrientacaoAtual == SUL){
            virarDireita();
            virarDireita();
            avanca();
        }else if( mOrientacaoAtual == LESTE){
            virarEsquerda();
            avanca();

        }else if( mOrientacaoAtual == OESTE){
            virarDireita();
            avanca();
        }
        mOrientacaoAtual = NORTE;
    }

    private void andarSul() {
        mY -= 1;
        if(mOrientacaoAtual == NORTE){
            virarEsquerda();
            virarEsquerda();
            avanca();
        }else if(mOrientacaoAtual == SUL){
            avanca();
        }else if( mOrientacaoAtual == LESTE){
            virarDireita();
            avanca();
        }else if( mOrientacaoAtual == OESTE){
            virarEsquerda();
            avanca();
        }
        mOrientacaoAtual = SUL;
    }

    private int checarMenorVizinho() {
        int menorValor = Integer.MAX_VALUE;
        int direcao = -1;

        //direita
        if(mX+1 < mMundo[mY].length-1 ) {
            if (mMundo[mY][mX + 1] != -1 && mMundo[mY][mX + 1] < menorValor) {
                direcao = LESTE;
                menorValor = mMundo[mY][mX + 1];
            }

        }
        //abaixo
        if(mY > 0 ){
            if (mMundo[mY-1][mX] != -1 && mMundo[mY-1][mX] < menorValor) {
                direcao = SUL;
                menorValor = mMundo[mY-1][mX];
            }
        }
        //esquerda
        if(mX > 0 ){
            if (mMundo[mY][mX-1] != -1 && mMundo[mY][mX-1] < menorValor) {
                direcao = OESTE;
                menorValor = mMundo[mY][mX-1];
            }
        }
        //acima
        if(mY < mMundo.length-1){
            if (mMundo[mY+1][mX] != -1 && mMundo[mY+1][mX] < menorValor) {
                direcao = NORTE;
                menorValor = mMundo[mY+1][mX];
            }
        }
        return direcao;
    }

    private void printMundo(){
        for(int i = 0; i < 20; i++){
            System.out.println();
        }
        for(int linha = mMundo.length-1; linha >=0; linha--){
            for(int coluna= 0; coluna < mMundo[linha].length; coluna++){
                System.out.print(( mMundo[linha][coluna] == Integer.MAX_VALUE ? "R" : mMundo[linha][coluna]) + "\t\t");
            }
            System.out.println();
        }
        System.out.println();
    }

    private void virarDireita() {
        /*mMotorDireita.rotate(ANGLE_ROTATE,true);
        mMotorEsquerda.rotate(-ANGLE_ROTATE);*/
    }

    private void avanca() {
        /*mMotorDireita.rotate(-MOVE_ROTATE, true);
        mMotorEsquerda.rotate(-MOVE_ROTATE);*/
    }

    private void virarEsquerda() {
        /*mMotorDireita.rotate(-ANGLE_ROTATE,true);
        mMotorEsquerda.rotate(ANGLE_ROTATE);*/

    }

    public static void main(String[] args) {
        new Robo(new WaveFront()).start();
    }

}