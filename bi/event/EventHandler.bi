PLAY_IMMEDIATE:Integer8 <- Integer8(1);
PLAY_DELAY:Integer8 <- Integer8(2);
REPLAY_DISCARD_IMMEDIATE:Integer8 <- Integer8(3);
REPLAY_DISCARD_DELAY:Integer8 <- Integer8(4);
REPLAY_UPDATE:Integer8 <- Integer8(5);
REPLAY_DOWNDATE:Integer8 <- Integer8(6);

/**
 * Event handler.
 */
final class EventHandler {
  /**
   * Past events, if recording is enabled.
   */
  trace:Queue<Event>;

  /**
   * Future events, if replaying is enabled.
   */
  replay:Queue<Event>;

  /**
   * Is recording enabled?
   */
  auto record <- false;
    
  /**
   * Mode.
   */
  auto mode <- PLAY_DELAY;
  
  /**
   * Handle a sequence of events.
   *
   * Returns: Log-weight.
   */
  function handle(evt:Event!) -> Real {
    auto w <- 0.0;
    while evt? {
      w <- w + evt!.accept(this);
    }
    return w;
  }

  /**
   * Handle a factor event.
   *
   * - evt: The event.
   *
   * Returns: Log-weight.
   */
  function handle(evt:FactorEvent) -> Real {
    if record {
      trace.pushBack(evt);
    }
    if mode == REPLAY_DISCARD_DELAY || mode == REPLAY_DISCARD_IMMEDIATE ||
        mode == REPLAY_UPDATE || mode == REPLAY_DOWNDATE {
      next();
    }
    return evt.observe();
  }

  /**
   * Handle a random event.
   *
   * - evt: The event.
   *
   * Returns: Log-weight.
   */  
  function handle(evt:RandomEvent) -> Real {
    auto w <- 0.0;    
    if record {
      trace.pushBack(evt);
    }
    if evt.hasValue() {
      if mode == PLAY_DELAY || mode == PLAY_IMMEDIATE {
        w <- evt.observe();
      } else if mode == REPLAY_DISCARD_DELAY ||
          mode == REPLAY_DISCARD_IMMEDIATE ||
          mode == REPLAY_UPDATE {
        next();
        w <- evt.observe();
      } else if mode == REPLAY_DOWNDATE {
        evt.downdate(next());
      } else {
        assert false;
      }
    } else {
      if mode == PLAY_DELAY {
        evt.assume();
      } else if mode == PLAY_IMMEDIATE {
        evt.value();
      } else if mode == REPLAY_DISCARD_DELAY {
        next();
        evt.assume();
      } else if mode == REPLAY_DISCARD_IMMEDIATE {
        next();
        evt.value();
      } else if mode == REPLAY_UPDATE {
        evt.assumeUpdate(next());
      } else if mode == REPLAY_DOWNDATE {
        evt.assumeDowndate(next());
      } else {
        assert false;
      }
    }
    return w;
  }
  
  /**
   * Set the record flag.
   */
  function setRecord(record:Boolean) {
    this.record <- record;
  }
  
  /**
   * Set the play/replay mode.
   */
  function setMode(mode:Integer8) {
    assert PLAY_IMMEDIATE <= mode && mode <= REPLAY_DOWNDATE;
    this.mode <- mode;
  }
  
  /**
   * Remove and return the replay trace.
   */
  function takeReplay() -> Queue<Event> {
    empty:Queue<Event>;
    auto replay <- this.replay;
    this.replay <- empty;
    return replay;
  }
  
  /**
   * Set the replay trace.
   */
  function setReplay(replay:Queue<Event>) {
    this.replay <- replay;
  }

  /**
   * Remove and return the recorded trace.
   */
  function takeTrace() -> Queue<Event> {
    empty:Queue<Event>;
    auto trace <- this.trace;
    this.trace <- empty;
    return trace;
  }
  
  /**
   * Set the recorded trace.
   */
  function setTrace(trace:Queue<Event>) {
    this.trace <- trace;
  }

  /**
   * Remove and return the next event on the replay trace.
   */
  function next() -> Event {
    assert !replay.empty();
    auto evt <- replay.front();
    replay.popFront();
    return evt;
  }

  /**
   * Rewind the record trace for replay. Equivalent to
   * `this.setReplay(this.takeTrace())`.
   */
  function rewind() {
    setReplay(takeTrace());
  }
}
