package ru.fizteh.fivt.students.eltyshev.shell.Commands;

import ru.fizteh.fivt.students.eltyshev.shell.FileSystem;

import java.io.IOException;
import java.util.ArrayList;

public class CdCommand extends Command {
    public void executeCommand(String params) throws IOException {
        ArrayList<String> parameters = CommandParser.parseParams(params);
        if (parameters.size() > 1) {
            throw new IllegalArgumentException("Too many arguments");
        }
        if (parameters.size() > 0) {
            FileSystem.getInstance().setWorkingDirectory(parameters.get(0));
        }
    }

    protected void initCommand() {
        commandName = "cd";
        helpString = "cd <directory name>";
    }
}
