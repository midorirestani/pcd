import java.util.concurrent.Semaphore;

public class TrafficController {

    private Semaphore semaphore;

    public TrafficController () {
        semaphore = new Semaphore(1);
    }

    private static void nap(int ms) {
        try {
            Thread.sleep(ms);
        } catch (InterruptedException e) {}
    }

    private void onEnter() {
        try {
            Thread wait = new Thread(new Runnable() {
                public void run() {
                    while (!semaphore.tryAcquire()) {
                        nap(25);
                    }
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
        onEnter();
    }

    public void enterRight() {
        System.out.println("chegou right");
        onEnter();
    }

    public void leaveLeft() {
        System.out.println("saiu left");
        semaphore.release();
    }

    public void leaveRight() {
        System.out.println("saiu right");
        semaphore.release();
    }
}