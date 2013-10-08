package ru.fizteh.fivt.students.eltyshev.shell.Commands;

import ru.fizteh.fivt.students.eltyshev.shell.FileSystem;

public class PwdCommand extends Command {
    public void executeCommand(String params) {
        String path = FileSystem.getInstance().getWorkingDirectory();
        System.out.println(path);
    }

    protected void initCommand() {
        commandName = "pwd";
        helpString = "pwd";
    }
}
