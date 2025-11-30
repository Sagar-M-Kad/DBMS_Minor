package com.assignment.excelhandler;

import java.io.*;
import org.apache.poi.xssf.usermodel.*;
import java.util.Scanner;

public class ExcelReader {

    private String filePath;
    private XSSFWorkbook workbook;
    private XSSFSheet sheet;

    public ExcelReader(String filePath) {
        this.filePath = filePath;
        try (FileInputStream fis = new FileInputStream(filePath)) {
            workbook = new XSSFWorkbook(fis);
            sheet = workbook.getSheetAt(0);
        } catch (IOException e) {
            System.out.println("Error loading file: " + e.getMessage());
        }
    }

    public void displaySheet() {
        if (sheet == null) return;
        for (int i = 0; i <= sheet.getLastRowNum(); i++) {
            XSSFRow row = sheet.getRow(i);
            if (row == null) continue;
            for (int j = 0; j < row.getLastCellNum(); j++) {
                System.out.print(row.getCell(j) + "\t");
            }
            System.out.println();
        }
    }

    public void updateCell(int rowIndex, int colIndex, String newValue) {
        if (sheet == null) return;
        XSSFRow row = sheet.getRow(rowIndex);
        if (row == null) row = sheet.createRow(rowIndex);
        XSSFCell cell = row.getCell(colIndex);
        if (cell == null) cell = row.createCell(colIndex);
        cell.setCellValue(newValue);
        System.out.println("Cell (" + rowIndex + "," + colIndex + ") updated to: " + newValue);
    }

    public void saveAndClose() {
        try (FileOutputStream fos = new FileOutputStream(filePath)) {
            workbook.write(fos);
            workbook.close();
            System.out.println("Changes saved to " + filePath);
        } catch (IOException e) {
            System.out.println("Error saving file: " + e.getMessage());
        }
    }

    public static void main(String[] args) {
        Scanner sc=new Scanner(System.in);
        ExcelFileHandler handler = new ExcelFileHandler("C:\\Users\\sachi\\Desktop\\Sample.xlsx");
        System.out.println("Opened Sample.xlsx\n");
        int opt, row, col;
        String val;
        while(true) {
        System.out.println("Enter 1 to display\nEnter 2 to update\nEnter 3 to save and close");
        opt=sc.nextInt();
        switch(opt) {
        case 1:handler.displaySheet();
            break;
        case 2: System.out.println("Enter row, column, String(value to be updated)");
                row=sc.nextInt();
                col=sc.nextInt();
                val=sc.next();
                handler.updateCell(row, col, val);
            break;
        case 3: handler.saveAndClose();
            break;
        default:System.out.println("Invalid Choice. Try again");
        }
        if(opt==3) {
            break;
        }
        }
    }
}

