package ru.fizteh.fivt.students.eltyshev.shell.Commands;

import java.util.ArrayList;
import java.util.HashMap;

public class HelpCommand extends Command {

    public HelpCommand(ArrayList<Command> commands) {
        for (final Command command : commands) {
            this.commands.put(command.getCommandName(), command);
        }
    }

    public void executeCommand(String params) {
        if (params.length() > 0) {
            printInfo(params);
        } else {
            for (final String commandName : commands.keySet()) {
                printInfo(commandName);
            }
        }
    }

    public void initCommand() {
        commandName = "help";
        helpString = "help";
    }

    private void printInfo(String commandName) {
        if (!commands.containsKey(commandName)) {
            throw new IllegalArgumentException(String.format("'%s': command not found", commandName));
        }
        Command command = commands.get(commandName);
        System.out.println(command.getHelpString());
    }

    private HashMap<String, Command> commands = new HashMap<String, Command>();
}
