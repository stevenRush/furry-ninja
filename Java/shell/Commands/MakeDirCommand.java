package ru.fizteh.fivt.students.eltyshev.shell.Commands;

import ru.fizteh.fivt.students.eltyshev.shell.FileSystem;

import java.io.IOException;


import java.util.ArrayList;

public class MakeDirCommand extends Command {

    public void executeCommand(String params) throws IOException {

        if (CommandParser.getParametersCount(params) > 1) {
            throw new IllegalArgumentException("Too many arguments!");
        }
        FileSystem.getInstance().createDirectory(params);
    }

    protected void initCommand() {
        commandName = "mkdir";
        helpString = "mkdir <directory name>";
    }
}
