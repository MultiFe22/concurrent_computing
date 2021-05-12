class Data{
    private int[] data = new int[LabSix.dim];
    Data(){
        for (int i = 0; i <  LabSix.dim; i++){
            data[i] = 0;
        }
    }
    public void sum(int ini, int end){
        for(int i = ini; i < end; i++){
            data[i] += 1;
        }
    }

    public int check(){
        int count = 0;
        for (int i = 0; i<data.length; i++) if (data[i] == 1) count++;
        return count;
    }

}

class T extends Thread {
    private int id;
    Data array;
    public T(int tid, Data array) {
        this.id = tid;
        this.array = array;
        job();
    }

    public void job() {
        System.out.println("Thread " + this.id + " started");

        int blocSize = LabSix.dim/LabSix.numThreads; //size of each block
        int ini = id * blocSize; //initial element of the block
        int end; //last element (!computed) of the block
        if(id == LabSix.numThreads - 1) end = LabSix.dim;
        else end = ini + blocSize;

        array.sum(ini,end);

        System.out.println("Thread " + this.id + " ended");
    }
}

public class LabSix {
    static final int numThreads = 4;
    static final int dim = 1000000;

    public static void main(String[] args) {
        Thread[] threads = new Thread[numThreads];
        Data array = new Data();

        for (int i=0; i<threads.length; i++) {
            threads[i] = new T(i, array);
        }

        for (int i=0; i<threads.length; i++) {
            threads[i].start();
        }

        for (int i=0; i<threads.length; i++) {
            try { threads[i].join(); } catch (InterruptedException e) { return; }
        }

        if(array.check() == LabSix.dim) System.out.println("Correct");

    }
}
