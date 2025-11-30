# Java Excel Reader - Apache POI

This document explains how to read Excel files in Java using the Apache POI library, as demonstrated in `ExcelReader.java`.

## What is Apache POI?

**Apache POI** is a Java library for reading and writing Microsoft Office files, including:
- Excel (.xls, .xlsx)
- Word (.doc, .docx)
- PowerPoint (.ppt, .pptx)

For Excel files, POI provides classes to work with:
- **Workbook** - The entire Excel file
- **Sheet** - A single worksheet/tab
- **Row** - A row of cells
- **Cell** - An individual cell

---

## Excel File Structure

```
Workbook (Excel File)
  └── Sheet (Worksheet/Tab)
       └── Row (Row 0, 1, 2...)
            └── Cell (Column A, B, C...)
```

---

## Adding Apache POI Dependencies

### Using Maven
Add to `pom.xml`:
```xml
<dependencies>
    <dependency>
        <groupId>org.apache.poi</groupId>
        <artifactId>poi</artifactId>
        <version>5.2.3</version>
    </dependency>
    <dependency>
        <groupId>org.apache.poi</groupId>
        <artifactId>poi-ooxml</artifactId>
        <version>5.2.3</version>
    </dependency>
</dependencies>
```

### Manual Download
1. Download POI JARs from: https://poi.apache.org/download.html
2. Add to classpath when compiling/running

---

## How the Program Works

### 1. Open the Excel File
```java
FileInputStream fis = new FileInputStream(new File("sample_data.xlsx"));
Workbook workbook = new XSSFWorkbook(fis);
```
- `XSSFWorkbook` is for `.xlsx` files (Excel 2007+)
- For older `.xls` files, use `HSSFWorkbook`

### 2. Access a Sheet
```java
Sheet sheet = workbook.getSheetAt(0);  // Get first sheet (index 0)
// OR
Sheet sheet = workbook.getSheet("SheetName");  // Get by name
```

### 3. Iterate Through Rows
```java
Iterator<Row> rowIterator = sheet.iterator();
while (rowIterator.hasNext()) {
    Row row = rowIterator.next();
    // Process row
}
```

### 4. Iterate Through Cells
```java
Iterator<Cell> cellIterator = row.cellIterator();
while (cellIterator.hasNext()) {
    Cell cell = cellIterator.next();
    // Process cell
}
```

### 5. Read Cell Values by Type
```java
switch (cell.getCellType()) {
    case STRING:
        String text = cell.getStringCellValue();
        break;
    case NUMERIC:
        double number = cell.getNumericCellValue();
        break;
    case BOOLEAN:
        boolean bool = cell.getBooleanCellValue();
        break;
    case FORMULA:
        String formula = cell.getCellFormula();
        break;
}
```

### 6. Access Specific Cell
```java
Row row = sheet.getRow(1);        // Row 1 (0-indexed)
Cell cell = row.getCell(0);       // Column 0 (A)
String value = cell.getStringCellValue();
```

---

## Common Operations

| Operation | Code |
|-----------|------|
| Get total rows | `sheet.getPhysicalNumberOfRows()` |
| Get last row number | `sheet.getLastRowNum()` |
| Get total sheets | `workbook.getNumberOfSheets()` |
| Get sheet name | `sheet.getSheetName()` |
| Check if cell is blank | `cell.getCellType() == CellType.BLANK` |

---

## Running the Program

### Compile
```bash
javac -cp ".;poi-5.2.3.jar;poi-ooxml-5.2.3.jar;*" ExcelReader.java
```

### Run
```bash
java -cp ".;poi-5.2.3.jar;poi-ooxml-5.2.3.jar;*" ExcelReader
```

**Note**: On Linux/Mac, use `:` instead of `;` as the classpath separator.

---

## Sample Output
```
Reading Sheet: Students
Total Rows: 4
================================================================================
ID	|	Name	|	CGPA	|	
--------------------------------------------------------------------------------
1.0	|	Alice	|	8.5	|	
--------------------------------------------------------------------------------
2.0	|	Bob	|	7.8	|	
--------------------------------------------------------------------------------
3.0	|	Charlie	|	9.1	|	
--------------------------------------------------------------------------------

[SUCCESS] Excel file read successfully!
```
