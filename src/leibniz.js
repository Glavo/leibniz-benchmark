let fs = require('fs');

function calculate(rounds) {
    rounds += 2;

    let pi = 1.0;
    let x = 1.0;

    for (let i = 2; i < rounds; i++) {
        x *= -1;
        pi += (x / (2 * i - 1));
    }

    return pi * 4;
}

function now() {
    const hrTime = process.hrtime();
    return hrTime[0] * 1000000000 + hrTime[1];
}

let rounds = parseInt(fs.readFileSync('./rounds.txt', 'utf8'));

for (let i = 0; i < 10; i++) {
    const start = now();
    const pi = calculate(rounds);
    const end = now();

    const time = (end - start) / 1000_000.0;
    console.log(`Warmup Iteration ${i} (${time}ms): ${pi}`);
}

let totalTime = 0.0;

for (let i = 0; i < 10; i++) {
    const start = now();
    const pi = calculate(rounds);
    const end = now();

    const time = (end - start) / 1000_000.0;
    console.log(`Iteration ${i} (${time}ms): ${pi}`);
    totalTime += time;
}

console.log(`Average: ${totalTime / 10}ms`)
