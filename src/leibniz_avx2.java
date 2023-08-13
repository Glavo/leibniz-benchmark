import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import jdk.incubator.vector.DoubleVector;
import jdk.incubator.vector.VectorOperators;
import jdk.incubator.vector.VectorSpecies;

// javac --release 20 --enable-preview --add-modules=jdk.incubator.vector leibniz_avx2.java
// java --enable-preview --add-modules=jdk.incubator.vector leibniz_avx2
public class leibniz_avx2 {
    private static final VectorSpecies<Double> SPECIES = DoubleVector.SPECIES_256;

    private static double calculate(int rounds) {
        DoubleVector divConstants = DoubleVector.fromArray(SPECIES, new double[]{1.0, 3.0, 5.0, 7.0, 9.0, 11.0, 13.0, 15.0},0);
        DoubleVector aones = DoubleVector.fromArray(SPECIES, new double[]{1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0},0);
        DoubleVector ones = DoubleVector.broadcast(SPECIES, 1.0);

        final int vecLen = SPECIES.length();
        final int nVecGroups = rounds / vecLen;
    
        DoubleVector sumTarget = DoubleVector.broadcast(SPECIES, 0.0);
        final int dvecLen = vecLen*2;

        for(int vidx = 0; vidx < nVecGroups; ++vidx) {
          sumTarget = sumTarget.add(ones.div(divConstants.add(dvecLen * vidx).mul(aones)));
        }
    
        double pi = sumTarget.reduceLanes(VectorOperators.ADD);
        for(int idx = nVecGroups * vecLen; idx < rounds; ++idx) {
            final double x = 1.0 - (2.0 * (idx & 0x1));
            pi += (x / (1 + (2*idx)));
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
