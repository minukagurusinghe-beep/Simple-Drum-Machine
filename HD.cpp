#include "splashkit.h"
#include <string>
#include <fstream>
#include "utilities.h"

//declaring needed constants
const int NUM_TRACKS = 4;
const int NUM_STEPS = 16;
const int CELL_SIZE = 40;
const int GRID_LEFT = 180;
const int GRID_TOP = 100;
const int NUM_PRESETS = 4;
const int NUM_IO_BUTTONS = 2;
const string PATTERN_FILE = "saved_pattern.txt";

enum presets
{
    PRESET1,
    PRESET2,
    PRESET3,
    PRESET4
};


//creating structure to model drum tracks
struct Track
{
    string name;
};

//structure to model each preset button
struct Button
{
    int x;
    int y;
    int width;
    int height;
    string label;
    int preset_id;
};


//creating structure to model each cell
struct Cell
{
    bool active;
};

//creating arrays to model tracks and cells
Track tracks[NUM_TRACKS];
sound_effect preset_samples[NUM_PRESETS][NUM_TRACKS];
Cell grid[NUM_TRACKS][NUM_STEPS];
Button preset_buttons[NUM_PRESETS];
Button io_buttons[NUM_IO_BUTTONS];
string status_message = "";

void load_preset(int selectedPreset);
bool point_in_button(int px, int py, const Button& button);

//setting up the tracks with the correct sounds and names
void setup_tracks()
{
    string track_names[NUM_TRACKS] = {"Kick", "Snare", "Hat", "Clap"};
    string preset_names[NUM_PRESETS] = {"hiphop", "rock", "rnb", "lofi"};
    string paths[NUM_PRESETS][NUM_TRACKS] = {
        {"data/presets/hiphop/hiphop.kick.wav", "data/presets/hiphop/hiphop.snare.wav",
         "data/presets/hiphop/hiphop.hat.wav", "data/presets/hiphop/hiphop.clap.wav"},
        {"data/presets/rock/rock.kick.wav", "data/presets/rock/rock.snare.wav",
         "data/presets/rock/rock.hat.wav", "data/presets/rock/rock.clap.wav"},
        {"data/presets/rnb/rnb.kick.wav", "data/presets/rnb/rnb.snare.wav",
         "data/presets/rnb/rnb.hat.wav", "data/presets/rnb/rnb.clap.wav"},
        {"data/presets/lofi/lofi.kickwav.wav", "data/presets/lofi/lofi.snare.wav",
         "data/presets/lofi/lofi.hat.wav", "data/presets/lofi/lofi.clap.wav"}
    };

    for (int track = 0; track < NUM_TRACKS; track++)
    {
        tracks[track].name = track_names[track];

        for (int preset = 0; preset < NUM_PRESETS; preset++)
        {
            string sound_name = preset_names[preset] + "_" + track_names[track];
            preset_samples[preset][track] = load_sound_effect(sound_name, paths[preset][track]);
        }
    }
}

//initialising grid
void setup_grid()
{
    for (int row = 0; row < NUM_TRACKS; row++)
    {
        for (int col = 0; col < NUM_STEPS; col++)
        {
            grid[row][col].active = false;
        }
    }
}

bool save_pattern()
{
    std::ofstream output(PATTERN_FILE);
    if (!output.is_open())
    {
        return false;
    }

    for (int row = 0; row < NUM_TRACKS; row++)
    {
        for (int col = 0; col < NUM_STEPS; col++)
        {
            output << (grid[row][col].active ? 1 : 0) << ' ';
        }
        output << '\n';
    }

    return output.good();
}

bool load_pattern()
{
    std::ifstream input(PATTERN_FILE);
    if (!input.is_open())
    {
        return false;
    }

    bool loaded_grid[NUM_TRACKS][NUM_STEPS];
    for (int row = 0; row < NUM_TRACKS; row++)
    {
        for (int col = 0; col < NUM_STEPS; col++)
        {
            int value;
            if (!(input >> value) || (value != 0 && value != 1))
            {
                return false;
            }
            loaded_grid[row][col] = value == 1;
        }
    }

    for (int row = 0; row < NUM_TRACKS; row++)
    {
        for (int col = 0; col < NUM_STEPS; col++)
        {
            grid[row][col].active = loaded_grid[row][col];
        }
    }

    return true;
}

//drawinf grid
void draw_grid()
{
    for (int row = 0; row < NUM_TRACKS; row++)
    {
        draw_text(tracks[row].name, color_white(), GRID_LEFT - 110, GRID_TOP + row * CELL_SIZE + 12);

        for (int col = 0; col < NUM_STEPS; col++)
        {
            int x = GRID_LEFT + col * CELL_SIZE;
            int y = GRID_TOP + row * CELL_SIZE;
            color cell_colour = grid[row][col].active ? color_green() : color_gray();

            fill_rectangle(cell_colour, x, y, CELL_SIZE - 2, CELL_SIZE - 2);
            draw_rectangle(color_white(), x, y, CELL_SIZE - 2, CELL_SIZE - 2);
        }
    }
}

//detecting user mouse input
void handle_mouse_input(int& selected_preset)
{
    if (!mouse_clicked(LEFT_BUTTON))
    {
        return;
    }

    int col = (mouse_x() - GRID_LEFT) / CELL_SIZE;
    int row = (mouse_y() - GRID_TOP) / CELL_SIZE;

    for (int i = 0; i < NUM_PRESETS; i++)
    {
        Button button = preset_buttons[i];
        if (mouse_x() >= button.x && mouse_x() <= button.x + button.width &&
            mouse_y() >= button.y && mouse_y() <= button.y + button.height)
        {
            selected_preset = i;
            load_preset(selected_preset);
            return;
        }
    }

    if (point_in_button(mouse_x(), mouse_y(), io_buttons[0]))
    {
        status_message = save_pattern() ? "Pattern saved" : "Could not save pattern";
        return;
    }
    if (point_in_button(mouse_x(), mouse_y(), io_buttons[1]))
    {
        status_message = load_pattern() ? "Pattern loaded" : "Could not load pattern";
        return;
    }

    if (row >= 0 && row < NUM_TRACKS && col >= 0 && col < NUM_STEPS)
    {
        grid[row][col].active = !grid[row][col].active;
    }
}


//playback logic, loop plays sounds for all active cells
void play_step(int step, int selected_preset)
{
    for (int row = 0; row < NUM_TRACKS; row++)
    {
        if (grid[row][step].active)
        {
            play_sound_effect(preset_samples[selected_preset][row], 1.0);
        }
    }
}

void set_track_steps(int track, const int steps[], int count)
{
    for (int i = 0; i < count; i++)
    {
        grid[track][steps[i]].active = true;
    }
}

void load_preset(int selectedPreset)
{
    setup_grid();

    const int hiphop_kick[] = {0, 4, 8, 12};
    const int hiphop_snare[] = {4, 12};
    const int hiphop_hat[] = {1, 3, 5, 7, 9, 11, 13, 15};
    const int hiphop_clap[] = {6, 14};

    const int rock_kick[] = {0, 4, 8, 12};
    const int rock_snare[] = {6, 14};
    const int rock_hat[] = {1, 2, 3, 5, 7, 9, 11, 13, 15};
    const int rock_clap[] = {10};

    const int rnb_kick[] = {0, 4, 8, 12};
    const int rnb_snare[] = {2, 6, 10, 14};
    const int rnb_hat[] = {1, 3, 5, 7, 9, 11, 13, 15};
    const int rnb_clap[] = {8, 12};

    const int lofi_kick[] = {0, 4, 8, 12};
    const int lofi_snare[] = {4, 10, 12, 14};
    const int lofi_hat[] = {1, 3, 5, 7, 9, 11, 13, 15};
    const int lofi_clap[] = {6, 14};

    switch (selectedPreset)
    {
        case PRESET1:
            set_track_steps(0, hiphop_kick, 4);
            set_track_steps(1, hiphop_snare, 2);
            set_track_steps(2, hiphop_hat, 8);
            set_track_steps(3, hiphop_clap, 2);
            break;

        case PRESET2:
            set_track_steps(0, rock_kick, 4);
            set_track_steps(1, rock_snare, 2);
            set_track_steps(2, rock_hat, 9);
            set_track_steps(3, rock_clap, 1);
            break;

        case PRESET3:
            set_track_steps(0, rnb_kick, 4);
            set_track_steps(1, rnb_snare, 4);
            set_track_steps(2, rnb_hat, 8);
            set_track_steps(3, rnb_clap, 2);
            break;

        case PRESET4:
            set_track_steps(0, lofi_kick, 4);
            set_track_steps(1, lofi_snare, 4);
            set_track_steps(2, lofi_hat, 8);
            set_track_steps(3, lofi_clap, 2);
            break;

        default:
            setup_grid();
            break;
    }
}
void draw_preset_buttons(const Button buttons[], int count)
{
    for (int i = 0; i < count; i++)
    {
        Button b = buttons[i];
        fill_rectangle(color_light_gray(), b.x, b.y, b.width, b.height);
        draw_rectangle(color_white(), b.x, b.y, b.width, b.height);
        draw_text(b.label, color_black(), b.x + 10, b.y + 10);
    }
}
bool point_in_button(int px, int py, const Button& b)
{
    return px >= b.x &&
           px <= b.x + b.width &&
           py >= b.y &&
           py <= b.y + b.height;
}



int main()
{
    preset_buttons[0] = {30, 20, 100, 40, "Hip-hop", PRESET1};
    preset_buttons[1] = {150, 20, 100, 40, "Rock", PRESET2};
    preset_buttons[2] = {270, 20, 100, 40, "R&B", PRESET3};
    preset_buttons[3] = {390, 20, 100, 40, "Lo-fi", PRESET4};
    io_buttons[0] = {530, 20, 120, 40, "Save Pattern", -1};
    io_buttons[1] = {670, 20, 120, 40, "Load Pattern", -1};

    open_window("Drum Sequencer", 900, 420);
    setup_tracks();
    int selected_preset = PRESET1;
    load_preset(selected_preset);

    int current_step = 0;
    unsigned long last_time = 0;


    //creating a loop with a fixed time itnerval to create playback
    timer drum_timer = create_timer("drum_timer");
    start_timer(drum_timer);
    last_time = timer_ticks("drum_timer");

    while (!window_close_requested("Drum Sequencer"))
    {
        process_events();
        handle_mouse_input(selected_preset);

        if (timer_ticks("drum_timer") - last_time >= 200)
        {
            play_step(current_step, selected_preset);
            current_step = (current_step + 1) % NUM_STEPS;
            last_time = timer_ticks("drum_timer");
        }

        clear_screen(color_black());
        draw_preset_buttons(preset_buttons, NUM_PRESETS);
        draw_preset_buttons(io_buttons, NUM_IO_BUTTONS);
        draw_grid();
        draw_text(status_message, color_white(), 530, 70);
        refresh_screen();
    }

    return 0;
}
