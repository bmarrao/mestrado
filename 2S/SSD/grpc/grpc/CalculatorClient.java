/*
 * Copyright 2015 The gRPC Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package io.grpc.examples.calculator;

import io.grpc.Channel;
import io.grpc.ManagedChannel;
import io.grpc.ManagedChannelBuilder;
import io.grpc.StatusRuntimeException;
import java.util.concurrent.TimeUnit;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * A simple client that makes requests to the {@link CalculatorServer}.
 */
public class CalculatorClient {
  private static final Logger logger = Logger.getLogger(CalculatorClient.class.getName());

  private final CalculatorGrpc.CalculatorBlockingStub blockingStub;

  /** Construct client for accessing Calculator server using the existing channel. */
  public CalculatorClient(Channel channel) {
    // 'channel' here is a Channel, not a ManagedChannel, so it is not this code's responsibility to
    // shut it down.

    // Passing Channels to code makes code easier to test and makes it easier to reuse Channels.
    blockingStub = CalculatorGrpc.newBlockingStub(channel);
  }

  /** interact with server */
  public void doIt() {
    double v1 = 2.5333296;
    double v2 = -1.772591;
    CalculatorRequest request1 = CalculatorRequest.newBuilder().setValue1(v1).setValue2(v2).build();

    logger.info("Asking for a computation: " + v1 + " + " + v2);
    logger.info("Asking for a computation: " + v1 + " - " + v2);
    logger.info("Asking for a computation: " + v1 + " * " + v2);	           logger.info("Asking for a computation: " + v1 + " / " + v2);
    
    CalculatorReply reply1, reply2, reply3, reply4;
    try {
      reply1 = blockingStub.add(request1);
      reply2 = blockingStub.sub(request1);
      reply3 = blockingStub.mul(request1);
      reply4 = blockingStub.div(request1);     
    } catch (StatusRuntimeException e) {
      logger.log(Level.WARNING, "RPC failed: {0}", e.getStatus());
      return;
    }
    logger.info("add result received: " + reply1.getValue());
    logger.info("sub result received: " + reply2.getValue());
    logger.info("mul result received: " + reply3.getValue());
    logger.info("div result received: " + reply4.getValue());

    double v3 = 2.5333296;
    double v4 = 0.0;
    logger.info("Asking for a computation: " + v3 + " / " + v4);
    CalculatorRequest request2 = CalculatorRequest.newBuilder().setValue1(v3).setValue2(v4).build();
    CalculatorReply reply5;
    try {
      reply5 = blockingStub.div(request2);
    } catch (StatusRuntimeException e) {
      logger.log(Level.WARNING, "RPC failed: {0}", e.getStatus());
      return;
    }
    logger.info("div by zero result: " + reply5.getValue());
  }

  /**
   * Start the client
   */
  public static void main(String[] args) throws Exception {
    // default target given as host:port
    String target = "localhost:50051";

    if (args.length == 1) {
      target = args[0];
    }

    // Create a communication channel to the server, known as a Channel. Channels are thread-safe
    // and reusable. It is common to create channels at the beginning of your application and reuse
    // them until the application shuts down.
    ManagedChannel channel = ManagedChannelBuilder.forTarget(target)
        // Channels are secure by default (via SSL/TLS). For the example we disable TLS to avoid
        // needing certificates.
        .usePlaintext()
        .build();
    try {
      CalculatorClient client = new CalculatorClient(channel);
      client.doIt();
    } finally {
      // ManagedChannels use resources like threads and TCP connections. To prevent leaking these
      // resources the channel should be shut down when it will no longer be used. If it may be used
      // again leave it running.
      channel.shutdownNow().awaitTermination(5, TimeUnit.SECONDS);
    }
  }
}
