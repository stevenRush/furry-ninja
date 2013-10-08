package ru.fizteh.fivt.students.eltyshev.shell;

import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.HashMap;
import java.io.IOException;

import ru.fizteh.fivt.students.eltyshev.shell.Commands.*;

public class Shell {

    private HashMap<String, Command> commands = new HashMap<String, Command>();
    private String[] args;
    private String prompt = "$ ";

    public Shell(String[] Args) {
        this.args = Args;
    }

    public void start() throws IOException {
        if (args.length == 0) {
            startInteractive();
        } else {
            packageMode();
        }
    }

    public void setCommands(ArrayList<Command> commands) {
        for (final Command command : commands) {
            this.commands.put(command.getCommandName(), command);
        }
    }

    private void startInteractive() throws IOException {
        Scanner scanner = new Scanner(System.in);
        printPrompt();
        while (scanner.hasNext()) {
            String command = scanner.nextLine();
            String[] commands = CommandParser.parseCommands(command);
            for (final String com : commands) {
                processCommand(com);
            }
            printPrompt();
        }
    }

    private void packageMode() {
        StringBuilder sb = new StringBuilder();
        for (final String st : args) {
            sb.append(st + " ");
        }
        String[] commands = CommandParser.parseCommands(sb.toString());
        for (final String command : commands) {
            boolean status = processCommand(command);
            if (!status) {
                System.exit(-1);
            }
        }
    }

    private void printPrompt() {
        System.out.print(prompt);
    }

    private boolean processCommand(String command) {
        String commandName = CommandParser.getCommandName(command);
        String params = CommandParser.getParameters(command);
        if (!commands.containsKey(commandName)) {
            System.err.println(String.format("%s: command not found. Type help to get help", commandName));
            return false;
        }
        boolean status = true;
        try {
            commands.get(commandName).executeCommand(params);
        } catch (IllegalArgumentException e) {
            System.err.println(commandName + ": " + e.getMessage());
            status = false;
        } catch (IOException e) {
            System.err.println(commandName + ": " + e.getMessage());
            status = false;
        }
        return status;
    }
}
