[%bs.raw {|require('./Counter.css')|}];

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

let isFinished = state =>
  switch (state.minutes, state.seconds) {
  | (0, 0) => true
  | _ => false
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
  | Tick when isFinished(state) =>
    ReasonReact.SideEffects((self => self.send(Stop)))
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
      <span className="counter__minutes">
        (ReasonReact.stringToElement(pad(self.state.minutes)))
      </span>
      <span className="counter__divider">
        (ReasonReact.stringToElement(":"))
      </span>
      <span className="counter__seconds">
        (ReasonReact.stringToElement(pad(self.state.seconds)))
      </span>
      <div className="counter__actions">
        <div className="counter__actions--start">
          <button
            className="waves-effect waves-light btn-large green"
            onClick=(
              _event =>
                self.send(
                  Start(Js.Global.setInterval(() => self.send(Tick), 1000))
                )
            )>
            (ReasonReact.stringToElement("Start"))
          </button>
        </div>
        <div className="counter__actions--stop">
          <button
            className="waves-effect waves-light btn-large red"
            onClick=(_event => self.send(Stop))>
            (ReasonReact.stringToElement("Stop"))
          </button>
        </div>
        <div className="counter__actions--reset">
          <button
            className="waves-effect waves-light btn-large orange"
            onClick=(_event => self.send(Reset))>
            (ReasonReact.stringToElement("Reset"))
          </button>
        </div>
      </div>
    </div>
};