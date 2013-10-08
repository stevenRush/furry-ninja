package ru.fizteh.fivt.students.eltyshev.shell.Commands;

import ru.fizteh.fivt.students.eltyshev.shell.FileSystem;

import java.util.ArrayList;
import java.io.IOException;

public class CopyCommand extends Command {
    public void executeCommand(String params) throws IOException {
        ArrayList<String> parameters = CommandParser.parseParams(params);
        if (parameters.size() > 2) {
            throw new IllegalArgumentException("Too many arguments!");
        }
        FileSystem.getInstance().copyFiles(parameters.get(0), parameters.get(1));
    }

    public void initCommand() {
        commandName = "cp";
        helpString = "cp <source> <destination>";
    }
}
