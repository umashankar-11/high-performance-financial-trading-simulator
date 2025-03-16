<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Component-Based Architecture Example</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      margin: 0;
      padding: 0;
      background-color: #f9f9f9;
    }
    .app-container {
      display: flex;
      flex-direction: column;
      align-items: center;
      padding: 20px;
    }
    .header {
      font-size: 2rem;
      margin-bottom: 20px;
    }
    .task-container {
      width: 100%;
      max-width: 500px;
      margin-top: 20px;
    }
    .task-item {
      display: flex;
      justify-content: space-between;
      padding: 15px;
      margin: 5px 0;
      background-color: #ffffff;
      border: 1px solid #ddd;
      border-radius: 5px;
    }
    .task-item button {
      background-color: red;
      color: white;
      border: none;
      padding: 5px;
      cursor: pointer;
      border-radius: 5px;
    }
    .task-item button:hover {
      background-color: darkred;
    }
    .input-container {
      display: flex;
      justify-content: center;
      width: 100%;
      max-width: 500px;
    }
    .input-container input {
      padding: 10px;
      width: 80%;
      margin-right: 10px;
      border: 1px solid #ddd;
      border-radius: 5px;
    }
    .input-container button {
      padding: 10px;
      cursor: pointer;
      border: 1px solid #ddd;
      border-radius: 5px;
      background-color: #28a745;
      color: white;
    }
    .input-container button:hover {
      background-color: #218838;
    }
    .footer {
      font-size: 1rem;
      margin-top: 20px;
    }
  </style>
</head>
<body>
  <div id="app" class="app-container"></div>

  <script>
    function createElement(tag, attributes, children) {
      const element = document.createElement(tag);
      Object.keys(attributes).forEach(key => {
        element.setAttribute(key, attributes[key]);
      });
      if (children) {
        children.forEach(child => {
          if (typeof child === 'string') {
            child = document.createTextNode(child);
          }
          element.appendChild(child);
        });
      }
      return element;
    }

    class TaskItem {
      constructor(task, onDelete) {
        this.task = task;
        this.onDelete = onDelete;
      }

      render() {
        const taskItemElement = createElement('div', { class: 'task-item' }, [
          createElement('span', {}, [this.task.name]),
          createElement('button', {}, ['Delete'])
        ]);

        taskItemElement.querySelector('button').addEventListener('click', () => {
          this.onDelete(this.task.id);
        });

        return taskItemElement;
      }
    }

    class TaskList {
      constructor(tasks, onDeleteTask) {
        this.tasks = tasks;
        this.onDeleteTask = onDeleteTask;
      }

      render() {
        const taskListElement = createElement('div', { class: 'task-container' });
        this.tasks.forEach(task => {
          const taskItem = new TaskItem(task, this.onDeleteTask);
          taskListElement.appendChild(taskItem.render());
        });
        return taskListElement;
      }
    }

    class InputForm {
      constructor(onAddTask) {
        this.onAddTask = onAddTask;
      }

      render() {
        const inputContainer = createElement('div', { class: 'input-container' }, [
          createElement('input', { type: 'text', placeholder: 'Enter task name' }),
          createElement('button', {}, ['Add Task'])
        ]);

        const inputField = inputContainer.querySelector('input');
        const addButton = inputContainer.querySelector('button');

        addButton.addEventListener('click', () => {
          const taskName = inputField.value.trim();
          if (taskName) {
            this.onAddTask(taskName);
            inputField.value = '';
          }
        });

        return inputContainer;
      }
    }

    class Footer {
      render() {
        const footer = createElement('div', { class: 'footer' }, [
          'Task Manager App - 2025'
        ]);
        return footer;
      }
    }

    class App {
      constructor() {
        this.tasks = [];
      }

      addTask(taskName) {
        const task = {
          id: Date.now(),
          name: taskName
        };
        this.tasks.push(task);
        this.render();
      }

      deleteTask(taskId) {
        this.tasks = this.tasks.filter(task => task.id !== taskId);
        this.render();
      }

      render() {
        const appContainer = document.getElementById('app');
        appContainer.innerHTML = '';

        const header = createElement('div', { class: 'header' }, ['Task Manager']);
        appContainer.appendChild(header);

        const inputForm = new InputForm((taskName) => this.addTask(taskName));
        appContainer.appendChild(inputForm.render());

        const taskList = new TaskList(this.tasks, (taskId) => this.deleteTask(taskId));
        appContainer.appendChild(taskList.render());

        const footer = new Footer();
        appContainer.appendChild(footer.render());
      }
    }

    const app = new App();
    app.render();

    class EditTaskForm {
      constructor(task, onUpdateTask) {
        this.task = task;
        this.onUpdateTask = onUpdateTask;
      }

      render() {
        const editForm = createElement('div', { class: 'input-container' }, [
          createElement('input', { type: 'text', value: this.task.name }),
          createElement('button', {}, ['Update Task'])
        ]);

        const inputField = editForm.querySelector('input');
        const updateButton = editForm.querySelector('button');

        updateButton.addEventListener('click', () => {
          const newTaskName = inputField.value.trim();
          if (newTaskName && newTaskName !== this.task.name) {
            this.onUpdateTask(this.task.id, newTaskName);
          }
        });

        return editForm;
      }
    }

    class AppWithUpdate {
      constructor() {
        this.tasks = [];
      }

      addTask(taskName) {
        const task = {
          id: Date.now(),
          name: taskName
        };
        this.tasks.push(task);
        this.render();
      }

      updateTask(taskId, newTaskName) {
        const task = this.tasks.find(t => t.id === taskId);
        if (task) {
          task.name = newTaskName;
          this.render();
        }
      }

      deleteTask(taskId) {
        this.tasks = this.tasks.filter(task => task.id !== taskId);
        this.render();
      }

      render() {
        const appContainer = document.getElementById('app');
        appContainer.innerHTML = '';

        const header = createElement('div', { class: 'header' }, ['Task Manager']);
        appContainer.appendChild(header);

        const inputForm = new InputForm((taskName) => this.addTask(taskName));
        appContainer.appendChild(inputForm.render());

        const taskList = new TaskList(this.tasks, (taskId) => this.deleteTask(taskId));
        appContainer.appendChild(taskList.render());

        const editTaskForm = new EditTaskForm(
          this.tasks[0],
          (taskId, newTaskName) => this.updateTask(taskId, newTaskName)
        );
        appContainer.appendChild(editTaskForm.render());

        const footer = new Footer();
        appContainer.appendChild(footer.render());
      }
    }

    const appWithUpdate = new AppWithUpdate();
    appWithUpdate.render();

    class AppWithLocalStorage {
      constructor() {
        this.tasks = JSON.parse(localStorage.getItem('tasks')) || [];
      }

      addTask(taskName) {
        const task = {
          id: Date.now(),
          name: taskName
        };
        this.tasks.push(task);
        this.saveTasks();
        this.render();
      }

      deleteTask(taskId) {
        this.tasks = this.tasks.filter(task => task.id !== taskId);
        this.saveTasks();
        this.render();
      }

      saveTasks() {
        localStorage.setItem('tasks', JSON.stringify(this.tasks));
      }

      render() {
        const appContainer = document.getElementById('app');
        appContainer.innerHTML = '';

        const header = createElement('div', { class: 'header' }, ['Task Manager']);
        appContainer.appendChild(header);

        const inputForm = new InputForm((taskName) => this.addTask(taskName));
        appContainer.appendChild(inputForm.render());

        const taskList = new TaskList(this.tasks, (taskId) => this.deleteTask(taskId));
        appContainer.appendChild(taskList.render());

        const footer = new Footer();
        appContainer.appendChild(footer.render());
      }
    }

    const appWithLocalStorage = new AppWithLocalStorage();
    appWithLocalStorage.render();
  </script>
</body>
</html>
