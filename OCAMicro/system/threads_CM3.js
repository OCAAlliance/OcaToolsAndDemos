function decode_stack(sp)
{
  WScript.Echo("stack "+sp.toString(16)+"\n");
  var i;
  var a = new Array();

  for (i=4;i<12;i++)
    {
      a[i] = TargetInterface.peekWord(sp); 
      sp+=4;
    }
  for (i=0;i<4;i++)
    {
      a[i] = TargetInterface.peekWord(sp);  
      sp+=4;
    }
  a[12] = TargetInterface.peekWord(sp); 
  sp+=4;
  a[14] = TargetInterface.peekWord(sp);  
  sp+=4;
  a[15] = TargetInterface.peekWord(sp); 
  sp+=4;
  a[16] = TargetInterface.peekWord(sp); 
  sp+=4;
  if (a[16] & (1<<9)) // Stack has been 8-byte aligned
    sp += 4;
  a[13] = sp;

  return a;
}

function add_task(task, state)
{
  var tcb, task_name, current_task, regs;

  current_task = Debug.evaluate("pxCurrentTCB");
  tcb = Debug.evaluate("*(tskTCB *)" + task);

  task_name = Debug.evaluate("(char*)&(*(tskTCB *)" + task + ").pcTaskName[0]");
  task_name = "#" + tcb.uxTCBNumber + " \"" + task_name + "\"";

  if (task == current_task)
  {
    state = "executing";
    regs = [];
  }
  else
  {
    regs = decode_stack(tcb.pxTopOfStack);
  }

  Threads.add(task_name, tcb.uxPriority, state, regs);
}

function add_list(list, state)
{
  var i, index, item, task;

  if (list && list.uxNumberOfItems>0)
  {
    index = list.xListEnd.pxNext;

    for (i = 0; i < list.uxNumberOfItems; i++)
    {
      item = Debug.evaluate("*(xListItem *)" + index);

      task = item ? item.pvOwner : 0;

      if (task)
        add_task(task, state);

      index = item.pxNext;

      if (i > 10)
        break;
    }
  }
}

function update() 
{
  var i, list, lists, max_priority;

  Threads.clear();

  if( Debug.evaluate("pxCurrentTCB") == 0 )
    return;

  max_priority = Debug.evaluate("uxTopReadyPriority");  

  Threads.newqueue("Ready");
  for (i = max_priority; i >= 0; i--)
  {
     list = Debug.evaluate("pxReadyTasksLists[" + i + "]");
     add_list(list, "ready");
  }

  Threads.newqueue("Blocked");
  list = Debug.evaluate("pxDelayedTaskList");
  if (list)
  {
    list = Debug.evaluate("*(xList *)" + list);
    add_list(list, "blocked");
  }

  list = Debug.evaluate("pxOverflowDelayedTaskList");
  if (list)
  {
    list = Debug.evaluate("*(xList *)" + list);
    add_list(list, "blocked*");
  }

  Threads.newqueue("Suspended");
  list = Debug.evaluate("xSuspendedTaskList");
  if (list)
  {
    add_list(list, "suspended");
  } 

}

