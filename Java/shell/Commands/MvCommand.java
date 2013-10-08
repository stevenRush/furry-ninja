package ru.fizteh.fivt.students.eltyshev.shell.Commands;

import ru.fizteh.fivt.students.eltyshev.shell.FileSystem;

import java.io.IOException;
import java.util.ArrayList;

public class MvCommand extends Command {
    public void executeCommand(String params) throws IOException {
        ArrayList<String> parameters = CommandParser.parseParams(params);
        if (parameters.size() > 2) {
            throw new IllegalArgumentException("Too many arguments!");
        }
        FileSystem.getInstance().moveFiles(parameters.get(0), parameters.get(1));
    }

    protected void initCommand() {
        commandName = "mv";
        helpString = "mv <source> <destination>";
    }
}
