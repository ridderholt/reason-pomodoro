open ReasonReact;

type state = {
  minutes: int,
  seconds: int,
  timerId: option(Js.Global.intervalId)
};

type action =
  | Start(Js.Global.intervalId)
  | Stop
  | Reset
  | Tick;

let calculateTime = state =>
  switch (state.minutes, state.seconds) {
  | (_, 0) => {...state, minutes: state.minutes - 1, seconds: 59}
  | _ => {...state, seconds: state.seconds - 1}
  };

let clearTimer = timerId =>
  switch timerId {
  | Some(timer) => Js.Global.clearInterval(timer)
  | _ => ()
  };

let counterReducer = (action, state) =>
  switch action {
  | Start(timer) => ReasonReact.Update({...state, timerId: Some(timer)})
  | Stop =>
    ReasonReact.UpdateWithSideEffects(
      {...state, timerId: None},
      (_self => clearTimer(state.timerId))
    )
  | Reset =>
    ReasonReact.UpdateWithSideEffects(
      {...state, minutes: 25, seconds: 0},
      (self => self.send(Stop))
    )
  | Tick => ReasonReact.Update(calculateTime(state))
  };

let pad = n =>
  if (n <= 9) {
    "0" ++ string_of_int(n);
  } else {
    string_of_int(n);
  };

let component = ReasonReact.reducerComponent("counter");

let make = _children => {
  ...component,
  initialState: () => {minutes: 25, seconds: 0, timerId: None},
  reducer: counterReducer,
  render: self =>
    <div className="counter">
      <span className="counter__hours">
        (ReasonReact.stringToElement(pad(self.state.minutes)))
      </span>
      <span> (ReasonReact.stringToElement(":")) </span>
      <span className="counter__minutes">
        (ReasonReact.stringToElement(pad(self.state.seconds)))
      </span>
      <div>
        <button
          onClick=(
            _event =>
              self.send(
                Start(Js.Global.setInterval(() => self.send(Tick), 1000))
              )
          )>
          (ReasonReact.stringToElement("Start"))
        </button>
        <button onClick=(_event => self.send(Stop))>
          (ReasonReact.stringToElement("Stop"))
        </button>
        <button onClick=(_event => self.send(Reset))>
          (ReasonReact.stringToElement("Reset"))
        </button>
      </div>
    </div>
};