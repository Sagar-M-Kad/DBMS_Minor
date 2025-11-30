import java.io.*;
import java.util.*;

/**
 * Simple Excel Reader that works with CSV files
 * No external libraries required!
 * 
 * Note: Save your Excel file as CSV format (.csv) to use this program
 */
public class ExcelReader {
    
    public static void main(String[] args) {
        // Change this to your CSV file path
        String csvFilePath = "sample_data.csv";
        
        try {
            readCSVFile(csvFilePath);
        } catch (IOException e) {
            System.out.println("Error reading file: " + e.getMessage());
            System.out.println("\nMake sure to:");
            System.out.println("1. Create an Excel file");
            System.out.println("2. Save it as CSV format (.csv)");
            System.out.println("3. Place it in the same folder as this program");
        }
    }
    
    public static void readCSVFile(String filePath) throws IOException {
        File file = new File(filePath);
        
        if (!file.exists()) {
            System.out.println("[ERROR] File not found: " + filePath);
            System.out.println("Please create a CSV file with the following format:");
            System.out.println("ID,Name,CGPA");
            System.out.println("1,Alice,8.5");
            System.out.println("2,Bob,7.8");
            return;
        }
        
        BufferedReader br = new BufferedReader(new FileReader(file));
        String line;
        int rowNumber = 0;
        
        System.out.println("\n" + "=".repeat(80));
        System.out.println("Reading CSV File: " + filePath);
        System.out.println("=".repeat(80));
        
        // Read and display all rows
        while ((line = br.readLine()) != null) {
            rowNumber++;
            
            // Split by comma
            String[] values = line.split(",");
            
            // Print row number
            System.out.print("Row " + rowNumber + ":\t");
            
            // Print each cell value
            for (int i = 0; i < values.length; i++) {
                System.out.print(values[i].trim());
                if (i < values.length - 1) {
                    System.out.print("\t|\t");
                }
            }
            System.out.println();
            System.out.println("-".repeat(80));
        }
        
        br.close();
        
        System.out.println("\n[SUCCESS] Total rows read: " + rowNumber);
        
        // Demonstrate accessing specific cell
        demonstrateSpecificCellAccess(filePath);
    }
    
    // Demonstrate how to access a specific cell
    public static void demonstrateSpecificCellAccess(String filePath) throws IOException {
        System.out.println("\n--- Accessing Specific Cell (Row 2, Column 1) ---");
        
        BufferedReader br = new BufferedReader(new FileReader(filePath));
        String line;
        int currentRow = 0;
        int targetRow = 2;  // Row 2 (1-indexed)
        int targetCol = 1;  // Column 1 (0-indexed, so this is the 2nd column)
        
        while ((line = br.readLine()) != null) {
            currentRow++;
            
            if (currentRow == targetRow) {
                String[] values = line.split(",");
                if (targetCol < values.length) {
                    System.out.println("Value at Row " + targetRow + ", Column " + targetCol + ": " + values[targetCol].trim());
                }
                break;
            }
        }
        
        br.close();
    }
    
    // Method to search for a specific value
    public static void searchValue(String filePath, String searchTerm) throws IOException {
        System.out.println("\n--- Searching for: " + searchTerm + " ---");
        
        BufferedReader br = new BufferedReader(new FileReader(filePath));
        String line;
        int rowNumber = 0;
        boolean found = false;
        
        while ((line = br.readLine()) != null) {
            rowNumber++;
            
            if (line.toLowerCase().contains(searchTerm.toLowerCase())) {
                System.out.println("Found in Row " + rowNumber + ": " + line);
                found = true;
            }
        }
        
        if (!found) {
            System.out.println("Not found.");
        }
        
        br.close();
    }
}
