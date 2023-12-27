import tkinter as tk
from tkinter import messagebox, filedialog
import csv
import time

root = tk.Tk()
root.title("TuringAfis Analyzer")
root.geometry("1000x600")

data = []
game_field = [[None for _ in range(10)] for _ in range(3)]

columns = ['state', 'current_value', 'value_to_write', 'displacement', 'next_state']

entries = {col: tk.StringVar() for col in columns}

def validate_values(cv, vtw, disp):
    if cv not in ('0', '1', 'vide') or vtw not in ('0', '1', 'vide'):
        return "The values for current_value and value_to_write must be 0, 1, or vide."
    if disp not in ('gauche', 'droite'):
        return "The value for displacement must be 'gauche' or 'droite'."
    return None

def add_row():
    error_msg = validate_values(entries['current_value'].get(),
                                entries['value_to_write'].get(),
                                entries['displacement'].get())
    if error_msg:
        messagebox.showerror("Error", error_msg)
        return

    row_data = {col: var.get() for col, var in entries.items()}
    data.append(row_data)
    for var in entries.values():
        var.set('')

def save_to_csv():
    if not data:
        messagebox.showerror("Error", "No data to save.")
        return

    filename = filedialog.asksaveasfilename(
        defaultextension=".csv",
        filetypes=[("CSV files", "*.csv"), ("All files", "*.*")],
    )

    if filename:
        with open(filename, 'w', newline='') as csvfile:
            writer = csv.DictWriter(csvfile, fieldnames=columns)
            writer.writeheader()
            writer.writerows(data)
        messagebox.showinfo("Success", f"Data saved to {filename}")

def color_column(position, color="blue"):
    for r in range(3):
        if r * 10 + position + 1 < len(game_field_frame.winfo_children()):
            game_field_frame.winfo_children()[r * 10 + position + 1].config(bg=color)

def analyze_fsm():
    transitions = {row['state']: {} for row in data}
    for row in data:
        transitions[row['state']][row['current_value']] = (row['value_to_write'], row['displacement'], row['next_state'])

    position = 0  # Start at column 1
    current_state = 'init'
    steps = 0  # To avoid infinite loops
    path = [f"case{position + 1}"]  # Start path from the initial position

    # Begin the simulation
    while current_state != 'fin' and steps < 100:
        # Determine the current value based on the cube's presence
        current_value = None
        for r, row in enumerate(game_field):
            if row[position] == '1':  # Check if the current position is occupied
                current_value = '0' if r == 1 else '1' if r == 0 else 'vide'
                break

        if current_value is None:
            messagebox.showinfo("Turing Analysis Result", f"Error: No cube found at column {position + 1}.")
            return

        # for missing instructions or undetermined next state
        if current_value not in transitions[current_state]:
            messagebox.showinfo("Turing Analysis Result", f"Missing instruction for state '{current_state}' with value '{current_value}'.")
            return

        value_to_write, displacement, next_state = transitions[current_state][current_value]

        # move the robot according to the displacement
        if displacement == 'droite' and position < 9:
            position += 1
        elif displacement == 'gauche' and position > 0:
            position -= 1
        else:
            messagebox.showinfo("Turing Analysis Result", f"Boundary issue detected at column {position + 1}.")
            return

        # Update the state and path
        current_state = next_state
        path.append(f"case{position + 1}")
        steps += 1

    # Display the result with the path taken
    if steps >= 100:
        messagebox.showinfo("Turing Analysis Result", f"Analysis terminated due to potential infinite loop after {steps} iterations.")
    else:
        path_str = " -> ".join(path)
        messagebox.showinfo("Turing Analysis Result", f"Turing analysis complete after {steps} iterations. Path taken: {path_str}")






def draw_game_field():
    game_field_frame = tk.Frame(root)
    game_field_frame.grid(row=5, column=0, columnspan=len(columns), pady=(10, 0), padx=20)

    # rows
    row_labels = ["vide", "0", "1"]
    for r, label in enumerate(row_labels):
        label_frame = tk.Label(game_field_frame, text=label, width=5)
        label_frame.grid(row=r, column=0, padx=2, pady=2)

    # columns
    for c in range(1, 11):  # 1 to 10
        label_frame = tk.Label(game_field_frame, text=f"case{c}", width=5)
        label_frame.grid(row=0, column=c, padx=2, pady=2)

    # cells
    for r in range(3):
        for c in range(1, 11):  # 1 to 10
            cell_frame = tk.Frame(game_field_frame, borderwidth=1, relief="raised", width=40, height=40, bg="white")
            cell_frame.grid(row=r, column=c, padx=2, pady=2)
            cell_frame.bind("<Button-1>", lambda event, row=r-1, col=c-1: toggle_cube(event, row, col))  # Adjusted indices




def toggle_cube(event, row, col):
    cell = event.widget
    if game_field[row][col] is None:
        cell.config(bg='black')  # Black indicates a cube is present
        game_field[row][col] = '1'  # Assuming '1' indicates presence of a cube
    else:
        cell.config(bg='white')
        game_field[row][col] = None

for i, column in enumerate(columns):
    tk.Label(root, text=column).grid(row=0, column=i, padx=5, pady=5)
    entry = tk.Entry(root, textvariable=entries[column])
    entry.grid(row=1, column=i, padx=5, pady=5)
    if i == len(columns) - 1:
        entry.bind("<Return>", add_row)


add_button = tk.Button(root, text="Add Row", command=add_row)
add_button.grid(row=2, column=0, columnspan=len(columns)//2, sticky=tk.W+tk.E, padx=5, pady=5)

save_button = tk.Button(root, text="Save to CSV", command=save_to_csv)
save_button.grid(row=2, column=len(columns)//2, columnspan=len(columns)//2, sticky=tk.W+tk.E, padx=5, pady=5)



analyze_button = tk.Button(root, text="Analyze FSM", command=analyze_fsm)
analyze_button.grid(row=3, columnspan=len(columns), pady=(10, 0))



draw_game_field()

root.mainloop()
