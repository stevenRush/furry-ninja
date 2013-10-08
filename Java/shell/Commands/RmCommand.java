package ru.fizteh.fivt.students.eltyshev.shell.Commands;

import ru.fizteh.fivt.students.eltyshev.shell.FileSystem;

import java.io.IOException;
import java.util.ArrayList;

public class RmCommand extends Command {
    public void executeCommand(String params) throws IOException {
        ArrayList<String> parameters = CommandParser.parseParams(params);
        FileSystem.getInstance().remove(parameters.get(0));
    }

    protected void initCommand() {
        commandName = "rm";
        helpString = "rm <file|directory>";
    }
}
