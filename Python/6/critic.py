from keras.layers import Input, Dense
from keras.models import Model
from keras.optimizers import Adam

# Number of hidden size.
HIDDEN_SIZE = 128
# Number of layers.
NUM_LAYERS = 2
# Number of epochs.
EPOCHS = 3
# Learning rate. Lower lr stabilises training greatly.
LR = 1e-4
# Batch size.
BATCH_SIZE = 64

class Critic:

    def __init__(self, num_state):
        self.critic = self.build_critic(num_state)


    def build_critic(self, num_state):

        state_input = Input(shape=(num_state,))
        x = Dense(HIDDEN_SIZE, activation='tanh')(state_input)
        for _ in range(NUM_LAYERS - 1):
            x = Dense(HIDDEN_SIZE, activation='tanh')(x)

        out_value = Dense(1)(x)

        model = Model(inputs=[state_input], outputs=[out_value])
        model.compile(optimizer=Adam(lr=LR), loss='mse')

        return model

    def fit(self, obs, reward):
        critic_loss = self.critic.fit([obs], [reward], batch_size=BATCH_SIZE, shuffle=True, epochs=EPOCHS, verbose=False)
        return critic_loss

    def predict(self, obs):
        return self.critic.predict(obs)
