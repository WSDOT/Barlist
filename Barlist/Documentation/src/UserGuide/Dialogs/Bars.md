Bar Definition{#dlg_bars}
=======
Bars are defined in the bar definition window.

The main window is continuously updated as bars are defined and modified.

## Groups
Use the drop down list to select a bar group.
A new group can be created by pressing [Add Group]

## Material
Use the drop down list to select the bar material.

## Basic Bar Description

Item | Description
-----|-----------
Location | Enter a descriptive location of the bar
Mark No. | Enter a mark number. The mark number is a 4-digit alpha-numeric parameter
Bend Type | Select the bend type from the drop down list. The bend guide shows the bend input parameters. (See note below for Type 90-99)
Size | Select the bar size from the drop down list.
No. Required | Enter of the total number of bars for this bar record
Use | Select the bar use type. The use type indicates the bend diameter and tail length for bends and hooks.
Flags | Check the flags that apply. See below for more information.

## Dimensions
Enter the bend dimensions. For US units, lengths are entered as "feet inches" (there is a space between feet and inches).

## Flags
Item | Description
-----|----------
Substructure | Indicates the bar is located in the substructure
Epoxy | Indicates the bar is epoxy coated
Varies | Indicate the length of the bars vary between two sets of bend dimensions
No. Each | Indicates the the number of bars at each length. Example: 10 bars, 2 at each length.

## Bar Record Status
The status of the bar record is indicated with a green, yellow, red status indicator. Status messages for warnings and errors are also provided.

## Command Buttons
Item | Description
----|------
[Add Bar] | The current data is used to create a new bar record in the current group
[Delete] | Deletes the current bar record
[Update] | Updates the current bar record with the current data
[Close] | Closes the bar definition window
[?] | Opens this help topic
[Disk] | Saves the barlist project

## Notes
> Type 90-99: Bend Types 90-99 are non-standard bends. Add a schematic of your non-standard bend to the Barlist drawing, using any of the dimension parameters except U. Because the bar is non-standard, the program does not know how to compute its length. Compute the length of one bar and enter it in the U field. The value entered into the U field is not provided in the Barlist drawing. The values entered into the W, X, Y, Z, T1, and T2 fields are shown in the Barlist drawing however, they are not used in the computations.
