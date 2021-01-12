import java.util.concurrent.Semaphore;

public class TrafficController {
    enum Traffic {
        RIGHT,
        LEFT,
        NONE
    }

    private Semaphore semaphore;
    private Traffic traffic;
    private int filaEsq;
    private int filaDir;

    public TrafficController () {
        traffic = Traffic.NONE;
        filaEsq = 0;
        filaDir = 0;
        semaphore = new Semaphore(1);
    }

    private static void nap(int ms) {
        try {
            Thread.sleep(ms);
        } catch (InterruptedException e) {}
    }

    private void onEnter(Traffic direction) {
        try {
            Thread wait = new Thread(new Runnable() {
                public void run() {
                    while (!semaphore.tryAcquire() && traffic != direction) {
                        nap(25);
                    }
                    traffic = direction;
                }
            });
            wait.start();
            wait.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public void enterLeft() {
        System.out.println("chegou left");
        filaEsq++;
        onEnter(Traffic.LEFT);
    }

    public void enterRight() {
        System.out.println("chegou right");
        filaDir++;
        onEnter(Traffic.RIGHT);
    }

    public void leaveLeft() {
        System.out.println("saiu left");
        filaDir--;
        if (filaDir == 0) {
            traffic = Traffic.NONE;
            semaphore.release();
        }
    }

    public void leaveRight() {
        System.out.println("saiu right");
        filaEsq--;
        if (filaEsq == 0) {
            traffic = Traffic.NONE;
            semaphore.release();
        }
    }
}