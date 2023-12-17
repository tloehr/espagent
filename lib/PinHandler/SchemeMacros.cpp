#include "SchemeMacros.h"

SchemeMacros::SchemeMacros()
{
    String scheme_macros = R"(
        {
        "very_long": {
            "repeat": 1,
            "scheme": [
            5000
            ]
        },
        "long": {
            "repeat": 1,
            "scheme": [
            2500
            ]
        },
        "medium": {
            "repeat": 1,
            "scheme": [
            1000
            ]
        },
        "short": {
            "repeat": 1,
            "scheme": [
            500
            ]
        },
        "very_short": {
            "repeat": 1,
            "scheme": [
            250
            ]
        },
        "very_slow": {
            "repeat": -1,
            "scheme": [
            1000,
            -5000
            ]
        },
        "slow": {
            "repeat": -1,
            "scheme": [
            1000,
            -2000
            ]
        },
        "normal": {
            "repeat": -1,
            "scheme": [
            1000,
            -1000
            ]
        },
        "fast": {
            "repeat": -1,
            "scheme": [
            500,
            -500
            ]
        },
        "very_fast": {
            "repeat": -1,
            "scheme": [
            250,
            -250
            ]
        },
        "netstatus": {
            "repeat": -1,
            "scheme": [
            250,
            -750
            ]
        },
        "single_buzz": {
            "repeat": 1,
            "scheme": [
            75,
            -75
            ]
        },
        "double_buzz": {
            "repeat": 2,
            "scheme": [
            75,
            -75
            ]
        },
        "triple_buzz": {
            "repeat": 3,
            "scheme": [
            75,
            -75
            ]
        },
        "annoying": {
            "repeat": -1,
            "scheme": [
            75,
            -75,
            75,
            -5000
            ]
        },
        "game_starts": {
            "repeat": 1,
            "scheme": [
            5000
            ]
        },
        "game_ends": {
            "repeat": 1,
            "scheme": [
            1500,
            -750,
            1500,
            -750,
            5000
            ]
        },
        "welcome_signal": {
            "wht": {
            "repeat": 100,
            "scheme": [
                250,
                -250,
                -250,
                -250,
                -250,
                -250,
                -250,
                -250
            ]
            },
            "red": {
            "repeat": 100,
            "scheme": [
                -250,
                250,
                -250,
                -250,
                -250,
                -250,
                -250,
                250
            ]
            },
            "ylw": {
            "repeat": 100,
            "scheme": [
                -250,
                -250,
                250,
                -250,
                -250,
                -250,
                250,
                -250
            ]
            },
            "grn": {
            "repeat": 100,
            "scheme": [
                -250,
                -250,
                -250,
                250,
                -250,
                250,
                -250,
                -250
            ]
            },
            "blu": {
            "repeat": 100,
            "scheme": [
                -250,
                -250,
                -250,
                -250,
                250,
                -250,
                -250,
                -250
            ]
            }
        },
        "fc_respawn_signal": {
            "buzzer": {
            "repeat": 1,
            "scheme": [
                75,
                -500,
                75,
                -500,
                150,
                -500,
                1200
            ]
            }
        },
        "signal_after_capture": {
            "led_all": "off",
            "red": {
            "repeat": 10,
            "scheme": [
                250,
                -250
            ]
            }
        },
        "prepare_team_signal": {
            "buzzer": {
            "repeat": 1,
            "scheme": [
                75,
                -200,
                400,
                -75,
                100
            ]
            }
        },
        "team_ready_signal": {
            "buzzer": {
            "repeat": 1,
            "scheme": [
                75,
                -100,
                400
            ]
            }
        },
        "team_hurry_up_signal": {
            "buzzer": {
            "repeat": -1,
            "scheme": [
                75,
                -75,
                75,
                -5000
            ]
            }
        }
        }
    )";

    DeserializationError jsonError = deserializeJson(json, scheme_macros);

    macros->put("very_long", R"(
    {
    "repeat": 1,
    "scheme": [
      5000
    ]
    })");

    macros->put("long", R"(
    {
    "repeat": 1,
    "scheme": [
      2500
    ]
    })");

    macros->put("welcome_signal", R"(
        {
            "wht": {
            "repeat": 100,
            "scheme": [
                250,
                -250,
                -250,
                -250,
                -250,
                -250,
                -250,
                -250
            ]
            },
            "red": {
            "repeat": 100,
            "scheme": [
                -250,
                250,
                -250,
                -250,
                -250,
                -250,
                -250,
                250
            ]
            },
            "ylw": {
            "repeat": 100,
            "scheme": [
                -250,
                -250,
                250,
                -250,
                -250,
                -250,
                250,
                -250
            ]
            },
            "grn": {
            "repeat": 100,
            "scheme": [
                -250,
                -250,
                -250,
                250,
                -250,
                250,
                -250,
                -250
            ]
            },
            "blu": {
            "repeat": 100,
            "scheme": [
                -250,
                -250,
                -250,
                -250,
                250,
                -250,
                -250,
                -250
            ]
            }
        }
    )");

    macros->put("fc_respawn_signal", R"(
        {
            "buzzer": {
            "repeat": 1,
            "scheme": [
                75,
                -500,
                75,
                -500,
                150,
                -500,
                1200
            ]
            }
        }
    )");

    macros->put("signal_after_capture", R"(
         {
            "led_all": "off",
            "red": {
            "repeat": 10,
            "scheme": [
                250,
                -250
            ]
            }
        }
    )");

    macros->put("prepare_team_signal", R"(
        {
            "buzzer": {
            "repeat": 1,
            "scheme": [
                75,
                -200,
                400,
                -75,
                100
            ]
            }
        }
    )");

    macros->put("team_ready_signal", R"(
        {
            "buzzer": {
            "repeat": 1,
            "scheme": [
                75,
                -100,
                400
            ]
        }
  }
    )");

    macros->put("team_hurry_up_signal", R"(
        {
            "buzzer": {
            "repeat": -1,
            "scheme": [
                75,
                -75,
                75,
                -5000
            ]
            }
        }
    )");

    macros->put("very_long", R"(
        {
            "repeat": 1,
            "scheme": [
            5000
            ]
        }
    )");

    macros->put("long", R"(
        {
            "repeat": 1,
            "scheme": [
            2500
            ]
        }
    )");

    macros->put("medium", R"(
        {
            "repeat": 1,
            "scheme": [
            1000
            ]
        }
    )");

    macros->put("short", R"(
        {
            "repeat": 1,
            "scheme": [
            500
            ]
        }
    )");

    macros->put("very_short", R"(
        {
            "repeat": 1,
            "scheme": [
            250
            ]
        }
    )");

    macros->put("very_slow", R"(
        {
            "repeat": -1,
            "scheme": [
            1000,
            -5000
            ]
        }
    )");

    macros->put("slow", R"(
        {
            "repeat": -1,
            "scheme": [
            1000,
            -2000
            ]
        }
    )");

    macros->put("normal", R"(
        {
            "repeat": -1,
            "scheme": [
            1000,
            -1000
            ]
        }
    )");

    macros->put("fast", R"(
        {
            "repeat": -1,
            "scheme": [
            500,
            -500
            ]
        }
    )");

    macros->put("very_fast", R"(
        {
            "repeat": -1,
            "scheme": [
            250,
            -250
            ]
        }
    )");

    macros->put("netstatus", R"(
        {
            "repeat": -1,
            "scheme": [
            75,
            -75
            ]
        }
    )");

    macros->put("single_buzz", R"(
        {
            "repeat": 1,
            "scheme": [
            75,
            -75
            ]
        }
    )");

    macros->put("double_buzz", R"(
        {
            "repeat": 2,
            "scheme": [
            75,
            -75
            ]
        }
    )");

    macros->put("triple_buzz", R"(
        {
            "repeat": 3,
            "scheme": [
            75,
            -75
            ]
        }
    )");

    macros->put("annoying", R"(
        {
            "repeat": -1,
            "scheme": [
            75,
            -75,
            75,
            -5000
            ]
        }
    )");

    macros->put("game_starts", R"(
        {
            "repeat": 1,
            "scheme": [
            5000
            ]
        }
    )");

    macros->put("game_ends", R"(
        {
            "repeat": 1,
            "scheme": [
            1500,
            -750,
            1500,
            -750,
            5000
            ]
        }
    )");
}

bool SchemeMacros::has(String key)
{
    return json.containsKey(key);
}

JsonObject SchemeMacros::get_macro(String key)
{
    return json[key];
}