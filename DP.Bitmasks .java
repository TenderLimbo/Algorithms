//package com.company;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.math.BigInteger;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Scanner;

// Условие

// Коридор длины n метров и ширины m метров разбит ровно на n × m клеток (квадратов 1 × 1).
// Необходимо покрыть его плитами 1 × 2 метра (и, соответственно, 2 × 1) без пропусков и
// наложений. Некоторые «клетки» первоначально могут быть заняты, т. е. их покрывать плитами
// не надо, но и накладывать на них плиты тоже нельзя. Необходимо подсчитать число различных 
// cпособов укладки паркета.

public class Main {

    public static void calc(int x, int y, int mask, int next_mask, int n, int m, BigInteger[][] dp) {
        if (x == n)
            return;
        if (y >= m)
            dp[x + 1][next_mask] = dp[x + 1][next_mask].add(dp[x][mask]);
        else {
            int my_mask = 1 << y;
            if ((mask & my_mask) != 0)
                calc(x, y + 1, mask, next_mask, n, m, dp);
            else {
                if ((next_mask & my_mask) == 0)
                    calc(x, y + 1, mask, next_mask | my_mask, n, m, dp);
                if (y + 1 < m && (mask & my_mask) == 0 && ((mask & (my_mask << 1)) == 0))
                    calc(x, y + 2, mask, next_mask, n, m, dp);
            }
        }
    }

    public static void main(String[] args) throws IOException {

        Scanner sc = new Scanner(new File("input.txt"));
        FileWriter fw = new FileWriter("output.txt");

        int n = sc.nextInt();
        int m = sc.nextInt();

        BigInteger[][] dp = new BigInteger[n + 1][1 << m];
        for (int x = 0; x <= n; ++x) {
            for (int mask = 0; mask < (1 << m); ++mask) {
                dp[x][mask] = BigInteger.valueOf(0);
            }
        }


        int[] f = new int[n + 1];

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                f[i] += (sc.nextInt() << j);
            }
        }

        dp[0][f[0]] = BigInteger.valueOf(1);

        for (int x = 0; x < n; ++x) {
            for (int mask = 0; mask < (1 << m); ++mask) {
                if (!dp[x][mask].equals(BigInteger.valueOf(0)))
                    calc(x, 0, mask, f[x + 1], n, m, dp);
            }
        }

        fw.write(dp[n][0].toString());


        fw.close();
        sc.close();

    }
}
