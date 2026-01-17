import pandas as pd
import os
import glob # Useful for finding files matching a pattern

# --- 1. Configuration ---
# IMPORTANT: Set this to the path of the folder containing your Excel files
folder_path = 'D:\coding\Study_code\HUAT-kerwin-labwork\数据分析与可视化\Touge\experiment\experiment1\data'

# Name for the output combined file
output_filename = 'combined_house_data.xlsx'
# Define the name for the new column to store the city/district
city_column_name = '城市'

# --- 2. Find all Excel (.xlsx) files in the folder ---
# Create the full search pattern
# os.path.join ensures the path works on different operating systems
file_pattern = os.path.join(folder_path, '*.xlsx')

# Use glob to find all files matching the pattern
xlsx_files = glob.glob(file_pattern)

# Check if any files were found
if not xlsx_files:
    print(f"错误：在文件夹 '{folder_path}' 中没有找到 .xlsx 文件。")
    print("请确保您已正确设置 'folder_path' 变量。")
    exit() # Exit if no files are found
else:
    print(f"在文件夹 '{folder_path}' 中找到 {len(xlsx_files)} 个 Excel 文件:")
    for f in xlsx_files:
        print(f"  - {os.path.basename(f)}") # Print found filenames

# --- 3. Read each file, add the city column, and store in a list ---
all_dataframes = [] # Initialize an empty list to store individual DataFrames

print("\n开始处理文件...")
for file_path in xlsx_files:
    filename_with_ext = os.path.basename(file_path)
    # Extract the city name from the filename (remove the '.xlsx' extension)
    city_name = os.path.splitext(filename_with_ext)[0]

    try:
        # Read the current Excel file into a DataFrame
        df_temp = pd.read_excel(file_path)

        # Add the new column with the extracted city name
        # This assigns the same city name to all rows in this temporary DataFrame
        df_temp[city_column_name] = city_name

        # Add the processed DataFrame to our list
        all_dataframes.append(df_temp)

        print(f"  - 成功处理: '{filename_with_ext}' (添加 '{city_column_name}': {city_name}, 读取行数: {len(df_temp)})")

    except FileNotFoundError:
        print(f"  - 错误: 文件 '{filename_with_ext}' 未找到 (可能在处理过程中被移动或删除)。")
    except Exception as e:
        # Catch other potential errors during file reading (e.g., corrupted file, permission issues)
        print(f"  - 错误: 处理文件 '{filename_with_ext}' 时发生错误: {e}")
        # You might choose to skip the file and continue, or stop execution
        # continue # Uncomment to skip this file and process the next one

# --- 4. Combine all DataFrames into one ---
if not all_dataframes:
    print("\n错误：没有成功读取任何文件的数据，无法合并。")
else:
    print(f"\n正在合并 {len(all_dataframes)} 个文件的数据...")
    # Use pandas.concat to merge all DataFrames in the list
    # ignore_index=True creates a new continuous index for the combined DataFrame
    combined_df = pd.concat(all_dataframes, ignore_index=True)

    print("合并完成！")
    print("\n合并后数据预览 (前5行):")
    print(combined_df.head())
    print("\n合并后数据预览 (后5行):")
    print(combined_df.tail())
    print("\n合并后数据信息:")
    combined_df.info() # Display summary info (columns, data types, non-null counts)

    # --- 5. Save the combined DataFrame to a new Excel file ---
    output_path = os.path.join(folder_path, output_filename)
    try:
        # index=False prevents pandas from writing the DataFrame index as a column in Excel
        combined_df.to_excel(output_path, index=False)
        print(f"\n成功将合并后的数据保存到: {output_path}")
    except Exception as e:
        print(f"\n错误: 保存合并后的文件到 '{output_path}' 时发生错误: {e}")