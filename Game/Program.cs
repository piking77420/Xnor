using Xnor;

Logger.OpenDefaultFile();

Window.Initialize();

Window.Show();

while (!Window.ShouldClose())
{
    Window.PollEvents();
}

Window.Shutdown();

Logger.Stop();
