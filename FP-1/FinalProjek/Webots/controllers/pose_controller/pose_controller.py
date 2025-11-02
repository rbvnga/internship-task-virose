from controller import Robot, Keyboard
import json, os, time
from enum import Enum, auto

# === INISIALISASI DASAR ===
robot = Robot()
timestep = int(robot.getBasicTimeStep())
keyboard = Keyboard()
keyboard.enable(timestep)

# === ENUM PROGRAM DAN MODE ===
class Program(Enum):
    pose_play = auto()
    motor_control = auto()

class Mode(Enum):
    Single = auto()
    Pair = auto()

class Action(Enum):
    Increment = auto()
    Decrement = auto()

# === KONFIGURASI DASAR ===
INCREMENT = 0.1
pose_folder = "../../poses/"
pose_files = [
    "pose-jongkok", "pose-berdiri", "pose-jalan-maju",
    "pose-jalan-mundur", "pose-geser-kanan", "pose-geser-kiri",
    "pose-belok-kanan", "pose-belok-kiri"
]
dict_keys = {k: ord(k) for k in "WSADEQZTRFC"}

# === DATA PENYIMPANAN ===
motors = {}
pos_sensors = {}
pose_data = {}
selection = 0
current_mode = Mode.Single
action_mode = Program.motor_control

# === SISTEM DEBOUNCE ===
last_press_time = {k: 0 for k in dict_keys.values()}
DEBOUNCE_TIME = 0.25  # detik antara input tombol yang sah

# === MOTOR LIST ===
motor_names = [
    "Head", "Neck",
    "ShoulderL", "ShoulderR",
    "ArmUpperL", "ArmUpperR",
    "ArmLowerL", "ArmLowerR",
    "PelvYL", "PelvYR",
    "PelvL", "PelvR",
    "LegUpperL", "LegUpperR",
    "LegLowerL", "LegLowerR",
    "AnkleL", "AnkleR",
    "FootL", "FootR"
]

# === NILAI POSE BERDIRI STABIL ===
pose_values = [
    0.19, 0.0, 0.387, -0.387, 0.77, -0.68, -0.04, 0.04,
    0.0, 0.0, -0.001, 0.0, 0.778, -0.779, -1.379, 1.378,
    -0.726, 0.716, 0.0, -0.0
]

# === FUNGSI UTILITAS ===
def take_input():
    key = keyboard.getKey()
    keys = set()
    while key != -1:
        keys.add(key)
        key = keyboard.getKey()
    return keys

def setup_motor():
    print(f"Initializing {len(motor_names)} motors...")
    for name in motor_names:
        device = robot.getDevice(name)
        if device:
            motors[name] = device
            sname = name + "S"
            sensor = robot.getDevice(sname)
            if sensor:
                sensor.enable(timestep)
                pos_sensors[name] = sensor
    print("[INFO] Semua motor siap.\n")

def set_initial_pose():
    print("=== Menyetel pose awal: BERDIRI ===")
    for i, name in enumerate(motor_names):
        if name in motors:
            motors[name].setPosition(pose_values[i])
            print(f"{name:12s} -> {pose_values[i]: .3f}")
    for _ in range(50):  # biarkan robot stabil di pose awal
        robot.step(timestep)
    print("[OK] Pose awal selesai.\n")

def load_motion():
    print("Memuat file JSON pose...")
    for fname in pose_files:
        fpath = os.path.join(pose_folder, fname + ".json")
        try:
            with open(fpath, "r") as f:
                data = json.load(f)
                for group in data["pose_group"]:
                    pose_data[group["nama"]] = {}
                    for p in group["pose"]:
                        pose_data[group["nama"]][p["nama"]] = p["posisi"]
            print(f"[OK] {fname}.json dimuat.")
        except Exception as e:
            print(f"[ERROR] Tidak bisa memuat {fname}.json: {e}")
    print()

def print_joint_list():
    print(f"\nIncrement: {INCREMENT:.2f}")
    for i, name in enumerate(motor_names):
        mark = "<== SELECTED" if i == selection else ""
        val = pos_sensors[name].getValue() if name in pos_sensors else 0.0
        print(f"[{i:2d}] {name:12s}: {val:.3f} {mark}")

def adjust_motor(act):
    if current_mode == Mode.Single:
        val = pos_sensors[motor_names[selection]].getValue()
        motors[motor_names[selection]].setPosition(val + (INCREMENT if act == Action.Increment else -INCREMENT))
    elif current_mode == Mode.Pair and selection + 1 < len(motor_names):
        left = pos_sensors[motor_names[selection]].getValue()
        right = pos_sensors[motor_names[selection + 1]].getValue()
        delta = INCREMENT if act == Action.Increment else -INCREMENT
        motors[motor_names[selection]].setPosition(left + delta)
        motors[motor_names[selection + 1]].setPosition(right - delta)

def write_motion(target, steps=10):
    if len(target) != len(motor_names):
        print("[WARN] Panjang pose tidak cocok jumlah motor!")
        return
    current = [pos_sensors[name].getValue() for name in motor_names]
    for s in range(steps):
        for i, name in enumerate(motor_names):
            pos = current[i] + (target[i] - current[i]) * ((s + 1) / steps)
            motors[name].setPosition(pos)
        robot.step(timestep)

# === SETUP AWAL ===
setup_motor()
set_initial_pose()
load_motion()

print("\n=== MODE: MOTOR CONTROL ===")

# === LOOP UTAMA ===
while robot.step(timestep) != -1:
    now = time.time()
    keys = take_input()

    # Ganti mode program
    if dict_keys["T"] in keys and now - last_press_time[dict_keys["T"]] > DEBOUNCE_TIME:
        action_mode = Program.pose_play if action_mode == Program.motor_control else Program.motor_control
        print(f"\n[MODE] Ganti ke: {action_mode.name}")
        last_press_time[dict_keys["T"]] = now

    # === MODE KONTROL MOTOR ===
    if action_mode == Program.motor_control:
        print_joint_list()

        # Navigasi naik/turun dengan debounce
        if dict_keys["W"] in keys and now - last_press_time[dict_keys["W"]] > DEBOUNCE_TIME:
            selection = max(0, selection - 1)
            last_press_time[dict_keys["W"]] = now

        elif dict_keys["S"] in keys and now - last_press_time[dict_keys["S"]] > DEBOUNCE_TIME:
            selection = min(len(motor_names) - 1, selection + 1)
            last_press_time[dict_keys["S"]] = now

        # Kontrol posisi sendi
        if dict_keys["A"] in keys:
            adjust_motor(Action.Decrement)
        if dict_keys["D"] in keys:
            adjust_motor(Action.Increment)

        # Ganti mode Single <-> Pair
        if dict_keys["C"] in keys and now - last_press_time[dict_keys["C"]] > DEBOUNCE_TIME:
            current_mode = Mode.Pair if current_mode == Mode.Single else Mode.Single
            print(f"[MODE] Ganti ke: {current_mode.name}")
            last_press_time[dict_keys["C"]] = now

        # Reset motor
        if dict_keys["Z"] in keys:
            motors[motor_names[selection]].setPosition(0.0)

        # Ubah kecepatan increment
        if dict_keys["Q"] in keys and now - last_press_time[dict_keys["Q"]] > DEBOUNCE_TIME:
            INCREMENT = max(0.01, INCREMENT - 0.01)
            print(f"[SET] INCREMENT: {INCREMENT:.2f}")
            last_press_time[dict_keys["Q"]] = now

        if dict_keys["E"] in keys and now - last_press_time[dict_keys["E"]] > DEBOUNCE_TIME:
            INCREMENT += 0.01
            print(f"[SET] INCREMENT: {INCREMENT:.2f}")
            last_press_time[dict_keys["E"]] = now

    # === MODE POSE PLAY ===
    elif action_mode == Program.pose_play:
        print("\n=== POSE PLAY MODE ===")
        for i, group in enumerate(pose_data.keys()):
            print(f"[{i}] {group}")
        print("[T] Kembali ke kontrol manual")
