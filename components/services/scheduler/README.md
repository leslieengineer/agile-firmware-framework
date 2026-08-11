# Scheduler Service Template

Use this directory only when several products need a shared scheduling policy. The scheduler defines tasks, periods, deadlines, and state transitions without depending directly on FreeRTOS or a vendor timer API.

For one product-specific main loop, keep the loop in `products/<product>/app/`. Add a reusable scheduler component only when reuse is real.
