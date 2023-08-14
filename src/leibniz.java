import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

public class leibniz {
    private static double calculate(int rounds) {
        rounds += 2;

        double pi = 1;
        double x = 1;

        for (int i = 2; i < rounds; i++) {
            x *= -1;
            pi += (x / (2 * i - 1));
        }

        return pi * 4;
    }

    public static void main(String[] args) throws IOException {
        int rounds = Integer.parseInt(Files.readString(Path.of("rounds.txt")));

        for (int i = 0; i < 10; i++) {
            long start = System.nanoTime();
            double pi = calculate(rounds);
            long end = System.nanoTime();
    
            double time = (end - start) / 1000_000.0;
            System.out.printf("Warmup Iteration %d (%.3fms): %.16f\n", i, time, pi);
        }
    
        double totalTime = 0;
    
        for (int i = 0; i < 10; i++) {
            long start = System.nanoTime();
            double pi = calculate(rounds);
            long end = System.nanoTime();
    
            double time = (end - start) / 1000_000.0;
            System.out.printf("Iteration %d (%.3fms): %.16f\n", i, time, pi);

            totalTime += time;
        }
        System.out.printf("Average: %fms\n", totalTime / 10);
    }
}
