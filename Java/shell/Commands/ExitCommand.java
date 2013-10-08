package ru.fizteh.fivt.students.eltyshev.shell.Commands;

public class ExitCommand extends Command {
    public void executeCommand(String params) {
        System.out.println("Good bye!");
        System.exit(0);
    }

    protected void initCommand() {
        commandName = "exit";
        helpString = "exit";
    }
}
