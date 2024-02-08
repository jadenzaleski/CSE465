import java.io.File;
import java.io.FileNotFoundException;
import java.nio.file.*;
import java.nio.file.spi.FileTypeDetector;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Scanner;

public class Zpm {
    static ArrayList<String[]> lines = new ArrayList<>();
    static HashMap<String, Object> hashMap = new HashMap<>();

    public static void main(String[] args) {
        if (args.length == 0) {
            System.out.println("[+] ERROR: You must provide a command line argument! (ex: java Zpm prog.zpm).");
            return;
        } else {
            System.out.print("[+] ARGS: ");
            for (String arg : args) {
                System.out.print(arg + ", ");
            }
            System.out.println();
        }

        for (String arg : args) {
            if (!isValidFile(arg)) {
                System.out.println("[+] ERROR: " + arg + " is not found and/or is not in the correct format! (ex: prog.zpm).");
                return;
            } else {
                System.out.println("[+] INFO: " + arg + " is found!");
                readFile(arg);
                parseFile();

            }
        }
    }

    private static boolean isValidFile(String name) {
        Path path = Paths.get(name);
        return Files.exists(path) && name.contains(".zpm");
    }

    private static void readFile(String path) {
        try {
            File f = new File(path);
            Scanner scanner = new Scanner(f);
            while (scanner.hasNextLine()) {
                String line = scanner.nextLine();
                // split up the line into an array of strings
                // ChatGPT came up with the regex which allows for strings with spaces to be put into one index (ex: "Hello World")
                String[] lineArray = line.split("\\s(?=(?:[^\"]*\"[^\"]*\")*[^\"]*$)");
                lines.add(lineArray);
                // remove quotes from the line
                for (int i = 0; i < lineArray.length; i++) {
                    lineArray[i] = lineArray[i].replaceAll("\"", "");
                }
            }
            scanner.close();
            System.out.println("[+] INFO: File reading complete.");
        } catch (FileNotFoundException e) {
            System.out.println("[+] ERROR: An error occurred while reading the file " + path + ".");
            e.printStackTrace();
        }
    }

    private static void parseFile() {
        for (int i = 0; i < lines.size(); i++) {
            String[] line = lines.get(i);
            switch (line[0]) {
                case "PRINT":
                    if (line[1].matches("[a-zA-Z]+") && hashMap.get(line[1]) != null) {
                        System.out.println(line[1] + "=" + hashMap.get(line[1]));
                    }
                    break;
                case "FOR":
                    break;
                case "ENDFOR":
                    // error
                    break;

                default:
                    // now we have 4 cases, var, operator, and value: A = 12 ;
                    if (line.length < 3 && line[line.length - 1].equals(";")) {
                        System.out.println("RUNTIME ERROR: line " + (i + 1));
                        System.exit(1);
                    }
                    // check and see if the var is letters and the value is either number or letters
                    if (line[0].matches("[a-zA-Z]+") && line[2].matches("^[a-zA-Z0-9\s]*$")) {
                        // check to see = is there
                        if (line[1].equals("=")) {
                            // if it's an int
                            if (line[2].matches("^[0-9]*$"))
                                hashMap.put(line[0], Integer.parseInt(line[2]));
                            else {
                                hashMap.put(line[0], line[2]);
                            }
                        }


                        if (line[1].equals("*=")) {
                            if (line[2].matches("^[0-9]*$") && hashMap.get(line[0]).toString().matches("^[0-9]*$")) {
                                // ChatGPT helped with casting
                                Integer old = (Integer) hashMap.get(line[0]);
                                if (old != null) {
                                    hashMap.put(line[0], old * Integer.parseInt(line[2]));
                                } else {
                                    System.out.println("RUNTIME ERROR: line " + (i + 1));
                                    System.exit(1);
                                }
                            } else {
                                System.out.println("RUNTIME ERROR: line " + (i + 1));
                                System.exit(1);
                            }
                        }

                        if (line[1].equals("+=")) {
                            if (line[2].matches("^[0-9]*$") && hashMap.get(line[0]).toString().matches("^[0-9]*$")) {
                                Integer old = (Integer) hashMap.get(line[0]);
                                if (old != null) {
                                    hashMap.put(line[0], old + Integer.parseInt(line[2]));
                                } else {
                                    System.out.println("RUNTIME ERROR: line " + (i + 1));
                                    System.exit(1);
                                }
                            } else if (line[2].matches(".*[a-zA-Z].*")) {
                                String old = hashMap.get(line[0]).toString();
                                if (hashMap.get(line[2]) == null) {
                                    if (old != null) {
                                        hashMap.put(line[0], old + line[2]);
                                    } else {
                                        System.out.println("RUNTIME ERROR: line " + (i + 1));
                                        System.exit(1);
                                    }
                                } else {
                                   if (old != null) {
                                        hashMap.put(line[0], (Integer.parseInt(old) + (int)hashMap.get(line[2])));
                                    } else {
                                        System.out.println("RUNTIME ERROR: line " + (i + 1));
                                        System.exit(1);
                                    }
                                }
                            } else {
                                System.out.println("RUNTIME ERROR: line " + (i + 1));
                                System.exit(1);
                            }
                        }

                        if (line[1].equals("-=")) {
                            if (line[2].matches("^[0-9]*$") && hashMap.get(line[0]).toString().matches("^[0-9]*$")) {
                                Integer old = (Integer) hashMap.get(line[0]);
                                if (old != null) {
                                    hashMap.put(line[0], old - Integer.parseInt(line[2]));
                                } else {
                                    System.out.println("RUNTIME ERROR: line " + (i + 1));
                                    System.exit(1);
                                }
                            } else {
                                System.out.println("RUNTIME ERROR: line " + (i + 1));
                                System.exit(1);
                            }

                        }
                    }
            }
        }
    }
}
