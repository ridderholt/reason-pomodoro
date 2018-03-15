[%bs.raw {|require('./app.css')|}];

let component = ReasonReact.statelessComponent("App");

let make = (~message, _children) => {
  ...component,
  render: _self =>
    <div className="App blue-grey lighten-5">
      <div className="App-header blue darken-4 z-depth-1">
        <h2> (ReasonReact.stringToElement(message)) </h2>
      </div>
      <div className="App-intro"> <Counter /> </div>
    </div>
};