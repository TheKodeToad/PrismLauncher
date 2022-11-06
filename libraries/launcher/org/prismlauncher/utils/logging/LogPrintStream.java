package org.prismlauncher.utils.logging;

import java.io.OutputStream;
import java.io.PrintStream;

/**
 * Used to create a print stream that redirects to Log.
 */
final class LogPrintStream extends PrintStream {

    private final Level level;

    public LogPrintStream(OutputStream out, Level level) {
        super(out);
        this.level = level;
    }

    @Override
    public void println(String x) {
        Log.log(x, this.level);
    }

    @Override
    public void println(Object x) {
        this.println(String.valueOf(x));
    }

    @Override
    public void println(boolean x) {
        this.println(String.valueOf(x));
    }

    @Override
    public void println(char x) {
        this.println(String.valueOf(x));
    }

    @Override
    public void println(int x) {
        this.println(String.valueOf(x));
    }

    @Override
    public void println(long x) {
        this.println(String.valueOf(x));
    }

    @Override
    public void println(float x) {
        this.println(String.valueOf(x));
    }

    @Override
    public void println(double x) {
        this.println(String.valueOf(x));
    }

    @Override
    public void println(char[] x) {
        this.println(String.valueOf(x));
    }

}