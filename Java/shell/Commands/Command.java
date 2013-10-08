package ru.fizteh.fivt.students.eltyshev.shell.Commands;

import ru.fizteh.fivt.students.eltyshev.shell.Shell;

import java.io.IOException;
import java.util.HashMap;

public abstract class Command {

    public Command() {
        initCommand();
    }

    public String getCommandName() {
        return commandName;
    }

    public String getHelpString() {
        return helpString;
    }

    public abstract void executeCommand(String params) throws IOException;

    protected abstract void initCommand();

    protected String commandName;
    protected String helpString;
}
