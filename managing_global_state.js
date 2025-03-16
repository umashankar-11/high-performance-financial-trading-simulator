<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Global State Management</title>
  <style>
    body {
      font-family: Arial, sans-serif;
    }
    .container {
      margin: 20px;
    }
    .counter {
      font-size: 2rem;
      margin-bottom: 20px;
    }
    .button {
      padding: 10px 20px;
      font-size: 1rem;
      cursor: pointer;
      margin: 5px;
    }
  </style>
</head>
<body>
  <div class="container">
    <div class="counter" id="counter">0</div>
    <button class="button" id="increment">Increment</button>
    <button class="button" id="decrement">Decrement</button>
    <button class="button" id="reset">Reset</button>
  </div>

  <script>
    class Store {
      constructor(reducer) {
        this.state = reducer(undefined, {});
        this.listeners = [];
      }

      getState() {
        return this.state;
      }

      dispatch(action) {
        this.state = this.reducer(this.state, action);
        this.listeners.forEach(listener => listener());
      }

      subscribe(listener) {
        this.listeners.push(listener);
      }

      replaceReducer(reducer) {
        this.reducer = reducer;
      }
    }

    function counterReducer(state = { count: 0 }, action) {
      switch (action.type) {
        case 'INCREMENT':
          return { ...state, count: state.count + 1 };
        case 'DECREMENT':
          return { ...state, count: state.count - 1 };
        case 'RESET':
          return { ...state, count: 0 };
        default:
          return state;
      }
    }

    const store = new Store(counterReducer);

    function updateCounter() {
      const state = store.getState();
      document.getElementById('counter').textContent = state.count;
    }

    store.subscribe(updateCounter);

    document.getElementById('increment').addEventListener('click', () => {
      store.dispatch({ type: 'INCREMENT' });
    });

    document.getElementById('decrement').addEventListener('click', () => {
      store.dispatch({ type: 'DECREMENT' });
    });

    document.getElementById('reset').addEventListener('click', () => {
      store.dispatch({ type: 'RESET' });
    });

    updateCounter();

    class GlobalStateManager {
      constructor() {
        this.store = store;
        this.init();
      }

      init() {
        this.store.subscribe(this.updateUI);
        this.setupEventListeners();
      }

      updateUI() {
        const state = this.store.getState();
        document.getElementById('counter').textContent = state.count;
      }

      setupEventListeners() {
        document.getElementById('increment').addEventListener('click', () => {
          this.store.dispatch({ type: 'INCREMENT' });
        });

        document.getElementById('decrement').addEventListener('click', () => {
          this.store.dispatch({ type: 'DECREMENT' });
        });

        document.getElementById('reset').addEventListener('click', () => {
          this.store.dispatch({ type: 'RESET' });
        });
      }
    }

    const globalState = new GlobalStateManager();

    function App() {
      const [state, setState] = useState({ count: 0 });

      const increment = () => setState(prevState => ({ count: prevState.count + 1 }));
      const decrement = () => setState(prevState => ({ count: prevState.count - 1 }));
      const reset = () => setState({ count: 0 });

      return (
        <div>
          <div className="counter">{state.count}</div>
          <button onClick={increment}>Increment</button>
          <button onClick={decrement}>Decrement</button>
          <button onClick={reset}>Reset</button>
        </div>
      );
    }

    function useState(initialState) {
      let state = initialState;
      const setState = (newState) => {
        state = { ...state, ...newState };
        return state;
      };
      return [state, setState];
    }

    function createStore(reducer) {
      let state;
      let listeners = [];

      const getState = () => state;

      const dispatch = (action) => {
        state = reducer(state, action);
        listeners.forEach(listener => listener());
      };

      const subscribe = (listener) => {
        listeners.push(listener);
      };

      state = reducer(state, {});
      return { getState, dispatch, subscribe };
    }

    function rootReducer(state = { count: 0 }, action) {
      switch (action.type) {
        case 'INCREMENT':
          return { count: state.count + 1 };
        case 'DECREMENT':
          return { count: state.count - 1 };
        case 'RESET':
          return { count: 0 };
        default:
          return state;
      }
    }

    const appStore = createStore(rootReducer);

    function updateUI() {
      const state = appStore.getState();
      document.getElementById('counter').textContent = state.count;
    }

    appStore.subscribe(updateUI);

    document.getElementById('increment').addEventListener('click', () => {
      appStore.dispatch({ type: 'INCREMENT' });
    });

    document.getElementById('decrement').addEventListener('click', () => {
      appStore.dispatch({ type: 'DECREMENT' });
    });

    document.getElementById('reset').addEventListener('click', () => {
      appStore.dispatch({ type: 'RESET' });
    });

    function applyMiddleware(middleware) {
      const store = createStore(rootReducer);
      const dispatch = store.dispatch;

      store.dispatch = (action) => {
        middleware(store)(dispatch)(action);
      };

      return store;
    }

    const loggerMiddleware = store => next => action => {
      console.log('dispatching', action);
      return next(action);
    };

    const storeWithMiddleware = applyMiddleware(loggerMiddleware);

    storeWithMiddleware.dispatch({ type: 'INCREMENT' });

    class StateManager {
      constructor(store) {
        this.store = store;
      }

      increment() {
        this.store.dispatch({ type: 'INCREMENT' });
      }

      decrement() {
        this.store.dispatch({ type: 'DECREMENT' });
      }

      reset() {
        this.store.dispatch({ type: 'RESET' });
      }

      getState() {
        return this.store.getState();
      }
    }

    const stateManager = new StateManager(storeWithMiddleware);

    stateManager.increment();
    stateManager.decrement();
    stateManager.reset();

    const AppWithStore = () => {
      const [count, setCount] = useState(0);

      return (
        <div>
          <div className="counter">{count}</div>
          <button onClick={() => setCount(count + 1)}>Increment</button>
          <button onClick={() => setCount(count - 1)}>Decrement</button>
          <button onClick={() => setCount(0)}>Reset</button>
        </div>
      );
    };
  </script>
</body>
</html>
