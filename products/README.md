# Products

A product is a complete firmware image, not a reusable library. It selects platform adapters, creates components, and connects them in `app/main.cpp`.

Do not put reusable sensor drivers or protocols here. Move those into `components/` so the next product can link them unchanged.
