package ru.fizteh.fivt.students.eltyshev.shell.Commands;

import ru.fizteh.fivt.students.eltyshev.shell.FileSystem;

import java.io.File;
import java.io.IOException;

public class DirCommand extends Command {

    public void executeCommand(String params) throws IOException {
        if (params.length() > 0) {
            throw new IllegalArgumentException("Too many arguments!");
        }
        String[] files = FileSystem.getInstance().listWorkingDirectory();
        if (files == null) {
            return;
        }
        for (final String file : files) {
            System.out.println(file);
        }
    }

    protected void initCommand() {
        commandName = "dir";
        helpString = "dir";
    }
}
